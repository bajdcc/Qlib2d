#include "stdafx.h"
#include "q2dhelper.h"


QSizeF Q2dHelper::mid;
bool Q2dHelper::mid_width;

Q2dHelper::Q2dHelper()
{
    world.set_helper(this);
    world.make_bound();
    world.init();
}


Q2dHelper::~Q2dHelper()
{
}

void Q2dHelper::paint(QPainter * painter, QPaintEvent * event, int elapsed)
{
    auto rect = event->rect();
    size = rect.size();
    mid = size * 0.5;
    mid_width = mid.width() >= mid.height();
    this->painter = painter;
    this->event = event;
    world.step(this);
}

void Q2dHelper::clear()
{
    painter->fillRect(event->rect(), background);
}

void Q2dHelper::paint_polygon(const std::vector<clib::v2>& v, PAINT_TYPE type)
{
    static std::vector<QPointF> vp;
    vp.clear();
    vp.resize(v.size());
    std::transform(v.begin(), v.end(), vp.begin(), world2screen);
    select(type);
    painter->drawPolygon(vp.data(), vp.size());
}

void Q2dHelper::paint_point(const clib::v2 & v, PAINT_TYPE type)
{
    select(type);
    painter->drawPoint(world2screen(v));
}

void Q2dHelper::paint_bound(const clib::v2 & a, const clib::v2 & b, PAINT_TYPE type)
{
    static std::vector<QPointF> vp;
    vp.resize(4);
    vp[0] = world2screen(clib::v2(a.x, a.y));
    vp[1] = world2screen(clib::v2(a.x, b.y));
    vp[2] = world2screen(clib::v2(b.x, b.y));
    vp[3] = world2screen(clib::v2(b.x, a.y));
    select(type);
    painter->drawLine(QLineF(vp[0], vp[1]));
    painter->drawLine(QLineF(vp[1], vp[2]));
    painter->drawLine(QLineF(vp[2], vp[3]));
    painter->drawLine(QLineF(vp[3], vp[0]));
}

void Q2dHelper::paint_line(const clib::v2 & a, const clib::v2 & b, PAINT_TYPE type)
{
    select(type);
    painter->drawLine(QLineF(world2screen(a), world2screen(b)));
}

void Q2dHelper::paint_line(const clib::v2 & a, const clib::v2 & b, const QColor & color)
{
    QPen pen(color);
    painter->setPen(pen);
    painter->drawLine(QLineF(world2screen(a), world2screen(b)));
}

void Q2dHelper::paint_circle(const clib::v2 & v, const qreal & r, PAINT_TYPE type)
{
    select(type);
    auto R = world2screen(clib::v2(r, 0)).x() - world2screen(clib::v2()).x();
    painter->drawEllipse(world2screen(v), R, R);
}

void Q2dHelper::paint_text(int x, int y, const QString & str, PAINT_TYPE type)
{
    painter->setPen(font);
    select(type);
    painter->drawText(x, y, str);
}

void Q2dHelper::paint_text(const clib::v2 & v, const qreal &angle, const QString & str, PAINT_TYPE type)
{
    painter->setPen(font);
    select(type);
    auto m = painter->fontMetrics();
    auto width = m.width(str);
    auto height = m.height();
    painter->translate(world2screen(v));
    painter->rotate(-180.0 * angle / M_PI);
    painter->drawText(-width / 2, height / 2, str);
    painter->resetMatrix();
}

clib::c2d_world & Q2dHelper::get_world()
{
    return world;
}

QSize Q2dHelper::get_size()
{
    return size;
}

clib::v2 Q2dHelper::screen2world(const QPointF & pt)
{
    if (mid_width)
        return clib::v2(pt.x() / mid.width() - 1.0, (mid.height() - pt.y() ) / mid.width());
    else
        return clib::v2((pt.x() - mid.width()) / mid.height(), 1.0 - pt.y() / mid.height());
}

QPointF Q2dHelper::world2screen(const clib::v2 & v)
{
    if (mid_width)
        return QPointF((v.x + 1.0) * mid.width(), -v.y * mid.width() + mid.height());
    else
        return QPointF(v.x * mid.height() + mid.width(), (-v.y + 1.0) * mid.height());
}

void Q2dHelper::select(PAINT_TYPE type)
{
    switch (type)
    {
    case Q2dHelper::Normal:
        painter->setPen(normal_border);
        break;
    case Q2dHelper::Static:
        painter->setPen(static_border);
        break;
    case Q2dHelper::Sleep:
        painter->setPen(sleep_border);
        break;
    case Q2dHelper::Center:
        painter->setPen(center_point);
        break;
    case Q2dHelper::Bound:
        painter->setPen(bound_border);
        break;
    case Q2dHelper::Collision:
        painter->setPen(collision_border);
        break;
    case Q2dHelper::Force:
        painter->setPen(force_line);
        break;
    case Q2dHelper::Velocity:
        painter->setPen(velocity_line);
        break;
    case Q2dHelper::Direction:
        painter->setPen(direction_line);
        break;
    case Q2dHelper::N:
        painter->setPen(collision_force);
        break;
    case Q2dHelper::Contact:
        painter->setPen(collision_contact);
        break;
    case Q2dHelper::DragLine:
        painter->setPen(drag_line);
        break;
    case Q2dHelper::DragPoint:
        painter->setPen(drag_point);
        break;
    case Q2dHelper::NormalText:
        painter->setFont(normal_text);
        break;
    case Q2dHelper::TitleText:
        painter->setFont(title_text);
        break;
    case Q2dHelper::CodeText:
        painter->setFont(code_text);
        break;
    default:
        break;
    }
}

void Q2dHelper::exec(QString &str)
{
    world.exec(str);
}
