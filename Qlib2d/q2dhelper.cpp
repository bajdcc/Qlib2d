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
    mid = rect.size() * 0.5;
    mid_width = mid.width() >= mid.height();
    this->painter = painter;
    this->event = event;
    painter->fillRect(event->rect(), background);
    world.step(this);
}

void Q2dHelper::paint_polygon(const std::vector<clib::v2>& v, PAINT_TYPE type)
{
    static std::vector<QPointF> vp;
    vp.clear();
    vp.resize(v.size());
    std::transform(v.begin(), v.end(), vp.begin(), trans);
    select_pen(type);
    painter->drawPolygon(vp.data(), vp.size());
}

void Q2dHelper::paint_point(const clib::v2 & v, PAINT_TYPE type)
{
    select_pen(type);
    painter->drawPoint(trans(v));
}

void Q2dHelper::paint_bound(const clib::v2 & a, const clib::v2 & b, PAINT_TYPE type)
{
    static std::vector<QPointF> vp;
    vp.resize(4);
    vp[0] = trans(clib::v2(a.x, a.y));
    vp[1] = trans(clib::v2(a.x, b.y));
    vp[2] = trans(clib::v2(b.x, b.y));
    vp[3] = trans(clib::v2(b.x, a.y));
    select_pen(type);
    painter->drawLine(QLineF(vp[0], vp[1]));
    painter->drawLine(QLineF(vp[1], vp[2]));
    painter->drawLine(QLineF(vp[2], vp[3]));
    painter->drawLine(QLineF(vp[3], vp[0]));
}

void Q2dHelper::paint_line(const clib::v2 & a, const clib::v2 & b, PAINT_TYPE type)
{
    select_pen(type);
    painter->drawLine(QLineF(trans(a), trans(b)));
}

void Q2dHelper::select_pen(PAINT_TYPE type)
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
    default:
        break;
    }
}

QPointF Q2dHelper::trans(const clib::v2 & v)
{
    if (mid_width)
        return QPointF((v.x + 1.0) * mid.width(), -v.y * mid.width() + mid.height());
    else
        return QPointF(v.x * mid.height() + mid.width(), (-v.y + 1.0) * mid.height());
}
