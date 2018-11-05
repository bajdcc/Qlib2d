//
// Project: clib2d
// Created by bajdcc
//

#include "stdafx.h"
#include "c2dworld.h"
#include "q2dhelper.h"
#include "cexception.h"
#include "cparser.h"
#include "csub.h"

namespace clib {

    QTime c2d_world::last_clock = QTime::currentTime();
    decimal c2d_world::dt = FRAME_SPAN;
    decimal c2d_world::dt_inv = 1.0 * FPS;
    QTime c2d_world::fpst;
    decimal c2d_world::fps = 1.0 * FPS;
    bool c2d_world::paused = false; // 是否暂停
    QString c2d_world::title("[TITLE]"); // 标题
    c2d_world *world = nullptr;

    QTime cycles_time = QTime::currentTime();
    int cycles = 0;
    qreal cycles_ps = 0.0;

    c2d_polygon *c2d_world::make_polygon(decimal mass, const std::vector<v2> &vertices, const v2 &pos, bool statics) {
        auto polygon = std::make_unique<c2d_polygon>(global_id++, mass, vertices);
        polygon->pos = pos;
        polygon->refresh();
        auto obj = polygon.get();
        if (statics) {
            polygon->mass.set(inf);
            polygon->statics = true;
            static_bodies.push_back(std::move(polygon));
        } else {
            bodies.push_back(std::move(polygon));
        }
        return obj;
    }

    c2d_polygon *c2d_world::make_rect(decimal mass, decimal w, decimal h, const v2 &pos, bool statics) {
        w = std::abs(w);
        h = std::abs(h);
        std::vector<v2> vertices = { // 设置四个顶点，逆时针
            {w / 2,  h / 2},
            {-w / 2, h / 2},
            {-w / 2, -h / 2},
            {w / 2,  -h / 2}
        };
        return make_polygon(mass, vertices, pos, statics);
    }

    c2d_circle *c2d_world::make_circle(decimal mass, decimal r, const v2 &pos, bool statics) {
        auto circle = std::make_unique<c2d_circle>(global_id++, mass, r);
        circle->pos = pos;
        auto obj = circle.get();
        if (statics) {
            circle->mass.set(inf);
            circle->statics = true;
            static_bodies.push_back(std::move(circle));
        } else {
            bodies.push_back(std::move(circle));
        }
        return obj;
    }

    c2d_revolute_joint *c2d_world::make_revolute_joint(c2d_body *a, c2d_body *b, const v2 &anchor) {
        auto joint = std::make_unique<c2d_revolute_joint>(a, b, anchor);
        auto obj = joint.get();
        joints.push_back(std::move(joint));
        return obj;
    }

    c2d_body *c2d_world::find_body(const v2 &pos) {
        auto body = std::find_if(bodies.begin(), bodies.end(), [&](auto &b) {
            return b->contains(pos);
        });
        if (body != bodies.end())
            return (*body).get();
        return nullptr;
    }

    uint32_t c2d_world::make_id(uint16_t a, uint16_t b) {
        return std::min(a, b) << 16 | (std::max(a, b));
    }

    bool c2d_world::collision_detection(const c2d_body::ptr &a, c2d_body::ptr &b) {
        auto bodyA = a.get();
        auto bodyB = b.get();
        auto id = make_id(bodyA->id, bodyB->id);
        auto _axis = 0;

        collision c;
        c.bodyA = bodyA;
        c.bodyB = bodyB;

        if (!AABB_collide(bodyA, bodyB) ||
            (((_axis = max_separating_axis(bodyA, bodyB, c.A)) != 1) ||
             (_axis == 2 ? true : (max_separating_axis(bodyB, bodyA, c.B) != 1)))) { // 是则不碰撞
            auto prev = collisions.find(id); // 查找下先前是否有碰撞
            if (prev != collisions.end()) { // 先前碰撞过，标记成不碰撞
                collisions.erase(prev); // 从碰撞数组中删掉
                bodyA->collision--; // 碰撞次数减一
                bodyB->collision--;
            }
            return false; // max_sa < 0 不相交
        }

        // 相交，产生碰撞
        auto prev = collisions.find(id); // 查找下先前是否有碰撞
        if (prev == collisions.end()) { // 之前没有产生过碰撞
            if (solve_collision(c)) { // 计算碰撞点
                collisions.insert(std::make_pair(id, c));
                // A和B标记成碰撞
                bodyA->collision++; // 碰撞次数加一
                bodyB->collision++;
#if ENABLE_SLEEP
                bodyA->sleep = false;
                bodyB->sleep = false;
#endif
            }
            return true;
        } else { // 先前产生过碰撞
            if (solve_collision(c)) { // 计算碰撞点
                clib::collision_update(c, collisions[id]);
                collisions[id] = c; // 替换碰撞结构
                return true;
            } else { // 没有碰撞
                collisions.erase(prev);
                bodyA->collision--; // 碰撞次数减一
                bodyB->collision--;
                return false;
            }
        }
    }

    decltype(auto) c2d_world::sleep_bodies() const {
#if ENABLE_SLEEP
        return std::count_if(bodies.begin(), bodies.end(), [&](auto &b) {
            return b->sleep;
        });
#else
        return 0;
#endif
    }

    void c2d_world::collision_detection() {
        auto size = bodies.size();
        for (size_t i = 0; i < size; i++) {
            if (bodies[i]->sleep) continue;
            for (size_t j = 0; j < size; j++) {
                if (bodies[j]->sleep || i < j)
                    collision_detection(bodies[i], bodies[j]);
            }
            for (auto &body : static_bodies) {
                collision_detection(bodies[i], body);
            }
        }
    }

    // https://github.com/erincatto/Box2D/blob/master/Box2D/Dynamics/Contacts/b2ContactSolver.cpp#L127
    // 碰撞计算准备

    void c2d_world::collision_prepare(collision & c) {
        static const auto kBiasFactor = COLL_BIAS; // 弹性碰撞系数
        const auto &a = *c.bodyA;
        const auto &b = *c.bodyB;
        auto tangent = c.N.normal(); // 接触面
                                     // 先计算好碰撞系数相关的量
        for (auto &contact : c.contacts) {
            auto nA = contact.ra.cross(c.N);
            auto nB = contact.rb.cross(c.N);
            auto kn = a.mass.inv + b.mass.inv +
                std::abs(a.inertia.inv) * nA * nA +
                std::abs(b.inertia.inv) * nB * nB;
            contact.mass_normal = kn > 0 ? COLL_NORMAL_SCALE / kn : 0.0;
            auto tA = contact.ra.cross(tangent);
            auto tB = contact.rb.cross(tangent);
            auto kt = a.mass.inv + b.mass.inv +
                std::abs(a.inertia.inv) * tA * tA +
                std::abs(b.inertia.inv) * tB * tB;
            contact.mass_tangent = kt > 0 ? COLL_TANGENT_SCALE / kt : 0.0;
            contact.bias = -kBiasFactor * dt_inv * std::min(0.0, contact.sep);
        }
    }

    void c2d_world::draw_collision(Q2dHelper * helper, const collision &c) {
        v2 ptA1, ptA2;
        const auto typeA = c.bodyA->type();
        const auto typeB = c.bodyB->type();
        auto showA = false;
        auto showB = false;
        if (!c.bodyA->statics && c.bodyA->text.isEmpty()) {
            if (typeA == C2D_POLYGON) {
                showA = true;
                auto bodyA = dynamic_cast<c2d_polygon *>(c.bodyA);
                ptA1 = bodyA->vertex(c.A.polygon.idx);
                ptA2 = bodyA->vertex(c.A.polygon.idx + 1);
                helper->paint_line(ptA1, ptA2, Q2dHelper::PAINT_TYPE::N);
            }
        }
        if (!c.bodyB->statics && c.bodyB->text.isEmpty()) {
            if (typeB == C2D_POLYGON) {
                showB = true;
                auto bodyB = dynamic_cast<c2d_polygon *>(c.bodyB);
                auto ptB1 = bodyB->vertex(c.B.polygon.idx);
                auto ptB2 = bodyB->vertex(c.B.polygon.idx + 1);
                helper->paint_line(ptB1, ptB2, Q2dHelper::PAINT_TYPE::N);
            }
        }
        if (showA && showB) {
            for (auto &contact : c.contacts) {
                helper->paint_point(contact.pos, Q2dHelper::PAINT_TYPE::Contact);
            }
        }
    }

    void c2d_world::collision_update(collision &c) {
        auto &a = *c.bodyA;
        auto &b = *c.bodyB;
        auto tangent = c.N.normal(); // 接触面
        for (auto &contact : c.contacts) {
            auto dv = (b.V + (-b.angleV * contact.rb.N())) -
                      (a.V + (-a.angleV * contact.ra.N()));

            // 法向力
            auto vn = dv.dot(c.N);
            auto dpn = (-vn + contact.bias) * contact.mass_normal;
            auto _pn = std::max(contact.pn + dpn, 0.0);
            dpn = _pn - contact.pn;
            contact.pn = _pn;

            a.update(gravity, 0);
            b.update(gravity, 0); // 初始化力和力矩

            auto p = dpn * c.N;
            a.impulse(-p, contact.ra);
            b.impulse(p, contact.rb);

            a.update(gravity, 1);
            b.update(gravity, 1); // 计算力和力矩，得出速度和角速度

            dv = (b.V + (-b.angleV * contact.rb.N())) -
                 (a.V + (-a.angleV * contact.ra.N()));

            // 切向力
            auto vt = dv.dot(tangent);
            auto dpt = -vt * contact.mass_tangent;
            auto friction = sqrt(a.f * b.f) * contact.pn;
            auto _pt = std::max(-friction, std::min(friction, contact.pt + dpt));
            dpt = _pt - contact.pt;
            contact.pt = _pt;

            a.update(gravity, 0);
            b.update(gravity, 0); // 初始化力和力矩

            p = dpt * tangent;
            a.impulse(-p, contact.ra);
            b.impulse(p, contact.rb);

            a.update(gravity, 1);
            b.update(gravity, 1); // 计算力和力矩，得出速度和角速度
        }
    }

#if ENABLE_SLEEP
    void c2d_world::collision_remove_sleep() {
        erase_if(collisions, [&](auto &c) {
            if (c.second.bodyA->statics)
                return c.second.bodyB->sleep;
            if (c.second.bodyB->statics)
                return c.second.bodyA->sleep;
            return c.second.bodyA->sleep && c.second.bodyB->sleep;
        });
    }
#endif

    void c2d_world::step(Q2dHelper * helper) {

        auto now = QTime::currentTime();
        // 计算每帧时间间隔
        dt = last_clock.msecsTo(now) * 0.001;
        dt = std::min(dt, FRAME_SPAN);
        dt_inv = 1.0 / dt;
        last_clock = now;
        auto fpsd = fpst.msecsTo(now) * 0.001;
        fps = 1.0 / fpsd;
        fpst = now;

        auto cycles_span = cycles_time.msecsTo(now);
        if (cycles_span > 1000) {
            cycles_ps = 1000 * cycles / cycles_span;
            cycles_time = now;
            cycles = 0;
        }

        if (!paused) {

            if (animation_id > 0)
                run_animation();

            collision_detection();

            // 碰撞预处理
            for (auto &col : collisions) {
                collision_prepare(col.second);
            }

            // 关节预处理
            for (auto &joint : joints) {
                joint->prepare(gravity);
            }

            for (auto &body : bodies)
                body->update(gravity, 4); // 合外力累计清零

            // 迭代十次，防止穿越
            for (auto i = 0; i < COLLISION_ITERATIONS; ++i) {

                // 碰撞处理
                for (auto &col : collisions) {
                    collision_update(col.second);
                }

                // 关节处理
                for (auto &joint : joints) {
                    joint->update(gravity);
                }
            }

            for (auto &body : bodies) {
                body->update(gravity, 0); // 初始化力和力矩
                body->update(gravity, 3); // 添加重力
                body->update(gravity, 1); // 计算力和力矩，得出速度和角速度
                body->update(gravity, 2); // 计算位移和角度
                body->update(gravity, 5); // 判定休眠
            }
        }

#if ENABLE_SLEEP
        collision_remove_sleep();
#endif

        helper->clear();

        for (auto &body : static_bodies) {
            body->draw(helper);
        }
        for (auto &body : bodies) {
            body->draw(helper);
        }
        for (auto &col : collisions) {
            draw_collision(helper, col.second);
        }
        for (auto &joint : joints) {
            joint->draw(helper);
        }

        if (mouse_drag) {
            auto dist = global_drag + global_drag_offset;
            helper->paint_line(global_drag, dist, Q2dHelper::PAINT_TYPE::DragLine);
            helper->paint_point(global_drag, Q2dHelper::PAINT_TYPE::DragPoint);
            helper->paint_point(dist, Q2dHelper::PAINT_TYPE::DragPoint);
            glLineWidth(1.0f);
            glColor3f(0.6f, 0.6f, 0.6f);
            glBegin(GL_LINES);
            glVertex2d(global_drag.x, global_drag.y);
            glVertex2d(global_drag.x + global_drag_offset.x, global_drag.y + global_drag_offset.y);
            glEnd();
            glColor3f(0.9f, 0.7f, 0.4f);
            glPointSize(4.0f);
            glBegin(GL_POINTS);
            glVertex2d(global_drag.x, global_drag.y);
            glVertex2d(global_drag.x + global_drag_offset.x, global_drag.y + global_drag_offset.y);
            glEnd();
        }

        auto size = helper->get_size();
        auto w = size.width();
        auto h = size.height();
        helper->paint_text(10, 20, "Qlib2d @bajdcc", Q2dHelper::PAINT_TYPE::NormalText);
        helper->paint_text(w - 140, 20, QString().sprintf("FPS: %.1f", fps), Q2dHelper::PAINT_TYPE::NormalText);
        helper->paint_text(w - 140, 40, QString().sprintf("IPS: %.1f", cycles_ps), Q2dHelper::PAINT_TYPE::NormalText);
        helper->paint_text(10, h - 20, "#c5p2", Q2dHelper::PAINT_TYPE::NormalText);
        helper->paint_text(w - 200, h - 20, QString::fromLocal8Bit("碰撞: %1, 休眠: %2")
            .arg(collisions.size()).arg(sleep_bodies()), Q2dHelper::PAINT_TYPE::NormalText);
        if (paused)
            helper->paint_text(w / 2 - 30, h - 20, QString::fromLocal8Bit("暂停"), Q2dHelper::PAINT_TYPE::NormalText);
        helper->paint_text(w / 2 - 200, (QApplication::desktop()->width() < 1920) ? 30 : 40, title, Q2dHelper::PAINT_TYPE::TitleText);

        if (animation_id != 0)
            helper->paint_text(20, 80, QString("[Now] %1").arg(animation_code), Q2dHelper::PAINT_TYPE::CodeText);
        if (!animation_queue.empty())
        {
            auto y = 90;
            auto i = 1;
            QString str;
            foreach(str, animation_queue) {
                if (i > 8)
                    break;
                helper->paint_text(20, y += 15, QString("[Queue #%1] %2").arg(i++).arg(str), Q2dHelper::PAINT_TYPE::CodeText);
            }
        }
    }

    void c2d_world::move(const v2 &v) {
        for (auto &body : bodies) {
#if ENABLE_SLEEP
            body->sleep = false;
#endif
            body->V += v;
        }
    }

    void c2d_world::rotate(decimal d) {
        for (auto &body : bodies) {
#if ENABLE_SLEEP
            body->sleep = false;
#endif
            body->angleV += d;
        }
    }

    void c2d_world::offset(const v2 &pt, const v2 &offset) {
        auto body = find_body(pt);
        if (body) {
#if ENABLE_SLEEP
            body->sleep = false;
#endif
            body->drag(pt, offset * body->mass.value);
        }
    }

    void c2d_world::mouse(const v2 &pt, bool down) {
        if (down) {
            mouse_drag = true;
            global_drag = pt;
            global_drag_offset.x = 0;
            global_drag_offset.y = 0;
        } else {
            mouse_drag = false;
            global_drag_offset.x = (pt.x - global_drag.x);
            global_drag_offset.y = (pt.y - global_drag.y);
            offset(global_drag, global_drag_offset);
            global_drag.x = pt.x;
            global_drag.y = pt.y;
        }
    }

    void c2d_world::motion(const v2 &pt) {
        if (mouse_drag) {
            global_drag_offset.x = (pt.x - global_drag.x);
            global_drag_offset.y = (pt.y - global_drag.y);
        }
    }

    void c2d_world::clear() {
        stop_animation();
        global_id = 1;
        bodies.clear();
        static_bodies.clear();
        collisions.clear();
        joints.clear();
    }

    void clib::c2d_world::reset()
    {
        bodies.clear();
        static_bodies.clear();
        collisions.clear();
        joints.clear();
    }

    void c2d_world::make_bound() {
        make_rect(inf, 1.65, 0.05, {0, -0.425}, true)->f = 0.8;
        make_rect(inf, 1.65, 0.05, {0, 0.425}, true)->f = 0.8;
        make_rect(inf, 0.05, 0.9, {0.8, 0}, true)->f = 0.8;
        make_rect(inf, 0.05, 0.9, {-0.8, 0}, true)->f = 0.8;
    }

    void c2d_world::scene(int id) {
        if (id != 7)
            clear();
        switch (id) {
            case 1: { // 一矩形、两三角形
                title = QString::fromLocal8Bit("【场景一】矩形与三角形");
                make_bound();
                std::vector<v2> vertices = {
                    {-0.05, 0},
                    {0.05,  0},
                    {0,     0.05}
                };
                make_polygon(2, vertices, {-0.05, -0.09})->f = 0.2;
                make_polygon(2, vertices, {0.05, -0.09})->f = 0.2;
                make_rect(2, 0.12, 0.2, {0, 0.085})->f = 0.2;
            }
                break;
            case 2: { // 堆叠的方块
                title = QString::fromLocal8Bit("【场景二】堆叠的方块");
                make_bound();
                std::default_random_engine e((uint32_t) time(nullptr));
                std::normal_distribution<decimal> dist{-0.01, 0.01};
                for (auto i = 0; i < 8; ++i) {
                    auto x = dist(e);
                    auto body = make_rect(1, 0.1, 0.08, {x, -0.2 + 0.08 * i});
                    body->f = 0.2;
                }
            }
                break;
            case 3: { // 金字塔
                title = QString::fromLocal8Bit("【场景三】金字塔");
                make_bound();
                v2 x{-0.2, -0.24};
                v2 y;
                int n = 8;
                for (auto i = 0; i < n; ++i) {
                    y = x;
                    for (auto j = i; j < n; ++j) {
                        make_rect(1, 0.08, 0.08, y)->f = 0.2;
                        y += {0.082, 0.0};
                    }
                    x += {0.041, 0.082};
                }
            }
                break;
            case 4: { // 牛顿摆
                title = QString::fromLocal8Bit("【场景四】牛顿摆");
                auto ground = make_rect(inf, 2, 0.02, {0, -0.5}, true);
                auto box1 = make_rect(100, 0.1, 0.1, {1.15, 0.6});
                box1->CO = 0.99;
                make_revolute_joint(ground, box1, {0.35, 0.6});
                for (size_t i = 0; i < 6; ++i) {
                    auto box2 = make_rect(100, 0.1, 0.1, {0.25 - i * 0.100001, -0.2});
                    box2->CO = 0.99;
                    make_revolute_joint(ground, box2, {0.25 - i * 0.100001, 0.6});
                }
            }
                break;
            case 5: { // 铰链
                title = QString::fromLocal8Bit("【场景五】铰链");
                auto ground = make_rect(1, 2, 0.04, {0, -0.45}, true);
                ground->f = 0.8;
                const auto mass = 10.0;
                const auto y = 0.45;
                auto last = ground;
                for (int i = 0; i < 10; ++i) {
                    auto box = make_rect(mass, 0.08, 0.02, {0.04 + 0.1 * i, y});
                    box->f = 0.4;
                    make_revolute_joint(last, box, {0.1 * i, y});
                    last = box;
                }
            }
                break;
            case 6: { // 金字塔（圆与多边形）
                title = QString::fromLocal8Bit("【场景六】金字塔（圆与多边形）");
                make_bound();
                v2 x{-0.4, -0.25};
                v2 y;
                int n = 8;
                std::default_random_engine e((uint32_t) time(nullptr));
                std::uniform_real_distribution<decimal> dist{0.03, 0.04};
                std::uniform_int_distribution<int> dist2{0, 4};
                for (auto i = 0; i < n; ++i) {
                    y = x;
                    for (auto j = i; j < n; ++j) {
                        switch (dist2(e)) {
                            case 1:
                                make_rect(1, 0.08, 0.08, y)->f = 0.2;
                                break;
                            case 2: {
                                static const auto sqrt_1_3 = 1 / std::sqrt(3);
                                static const std::vector<v2> vertices = {
                                    {0.04,  -0.04 * sqrt_1_3},
                                    {0,      0.04 * sqrt_1_3},
                                    {-0.04, -0.04 * sqrt_1_3}
                                };
                                make_polygon(1, vertices, y)->f = 0.2;
                            }
                                break;
                            case 3: {
                                static const auto sqrt_3 = std::sqrt(3);
                                static const std::vector<v2> vertices = {
                                    {0.04,  0},
                                    {0.02,  0.02 * sqrt_3},
                                    {-0.02, 0.02 * sqrt_3},
                                    {-0.04, 0},
                                    {-0.02, -0.02 * sqrt_3},
                                    {0.02,  -0.02 * sqrt_3},
                                };
                                make_polygon(1, vertices, y)->f = 0.2;
                            }
                                break;
                            default:
                                make_circle(1, dist(e), y)->f = 0.2;
                                break;
                        }
                        y += {0.1, 0.0};
                    }
                    x += {0.05125, 0.082};
                }
            }
                break;
            case 7: { // 脚本
                title = QString::fromLocal8Bit("【场景七】脚本");
                make_bound();
                start_animation(2);
            }
                break;
            case 8: { // 脚本
                title = QString::fromLocal8Bit("【场景八】特效");
                make_bound();
                start_animation(3);
            }
                    break;
            default: {
                title = QString::fromLocal8Bit("【默认场景】常见几何图形");
                make_bound();
                make_rect(1, 0.1, 0.1, {0, 0})->f = 0.2;
                make_circle(1, 0.05, {0.1, 0})->f = 0.2;
                static std::vector<v2> vertices = {
                    {0.0, 0.0},
                    {0.1, 0.0},
                    {0.0, 0.1}
                };
                make_polygon(1, vertices, {0, 0.1})->f = 0.2;
            }
                break;
        }
    }

    void c2d_world::init() {
        vm.set_world(this);
        scene(0);
    }

    void c2d_world::exec(QString & str)
    {
        if (animation_id == 0)
        {
            animation_code = str;
            start_animation(1);
            str.clear();
        }
        else
        {
            animation_queue.enqueue(str);
        }
    }

    void c2d_world::toggle_pause()
    {
        paused = !paused;
    }

    void c2d_world::toggle_gravity()
    {
        gravity.y = gravity.y < 0 ? 0 : GRAVITY;
        for (auto &body : bodies) {
#if ENABLE_SLEEP
            body->sleep = false;
#endif
        }
    }

    size_t c2d_world::get_collision_size() const {
        return collisions.size();
    }

    size_t c2d_world::get_sleeping_size() const {
        return sleep_bodies();
    }

    void c2d_world::invert_gravity() {
        gravity.y = gravity.y < 0 ? 0 : GRAVITY;
        for (auto &body : bodies) {
#if ENABLE_SLEEP
            body->sleep = false;
#endif
        }
    }

    void clib::c2d_world::set_gravity(const v2 & v)
    {
        gravity = v;
    }

    void clib::c2d_world::set_cycle(int cycle)
    {
        this->cycle = cycle;
    }

    void clib::c2d_world::record()
    {
        record_now = QTime::currentTime();
    }

    bool clib::c2d_world::reach(const decimal & d)
    {
        return record_now.addMSecs(d * 1000.0) < QTime::currentTime();
    }

    void c2d_world::set_helper(Q2dHelper * helper)
    {
        this->helper = helper;
    }

    void c2d_world::start_animation(uint32_t id) {
        if (animation_id != id) {
            if (id >= 1 && id <= 3) {
                if (id >= 2) {
                    animation_code = QString(R"(+ __author__ " " __project__)");
                    auto code = get_script(id);
                    QString c;
                    animation_queue.clear();
                    foreach(c, code.split("\n")) {
                        if (!c.isEmpty())
                            animation_queue.enqueue(c);
                    }
                }
                emit helper->output(QString("Running lisp...\n%1").arg(animation_code), 0);
                // vm.reset();
                try {
                    parser = std::make_unique<cparser>(std::string(animation_code.toLocal8Bit()));
                    auto node = parser->parse();
                    vm.prepare(node);
                } catch (const cexception &e) {
                    qDebug() << e.msg;
                    parser.reset(nullptr);
                    emit helper->output(QString(e.msg), 0);
                    return;
                }
                vm.save();
            }
            animation_id = id;
        }
    }

    void c2d_world::stop_animation() {
        if (animation_id != 0) {
            parser.reset(nullptr);
            animation_id = 0;
            if (!animation_queue.empty()) {
                animation_code = animation_queue.dequeue();
                start_animation(1);
            }
        }
    }

    void c2d_world::run_animation() {
        try {
            auto val = vm.run(cycle, cycles);
            if (val != nullptr) {
                std::stringstream ss;
                clib::cvm::print(val, ss);
                auto output = QString::fromLocal8Bit(ss.str().c_str());
                emit helper->output(output, 0);
                vm.gc();
                vm.dump();
                stop_animation();
            }
        } catch (const cexception &e) {
            qDebug() << e.msg;
            vm.restore();
            vm.gc();
            stop_animation();
            emit helper->output(QString(e.msg), 0);
        }
    }
}