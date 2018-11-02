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

void Q2dHelper::paint_polygon(const std::vector<clib::v2>& v, bool statics)
{
    std::vector<QPointF> vp;
    vp.resize(v.size());
    std::transform(v.begin(), v.end(), vp.begin(), trans);
    painter->setPen(static_border);
    painter->drawPolygon(vp.data(), vp.size());
}

QPointF Q2dHelper::trans(const clib::v2 & v)
{
    if (mid_width)
        return QPointF((v.x + 1.0) * mid.width(), v.y * mid.width() + mid.height());
    else
        return QPointF(v.x * mid.height() + mid.width(), (v.y + 1.0) * mid.height());
}
