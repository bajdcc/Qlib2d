#pragma once

#include "c2dworld.h"

class Q2dHelper
{
public:
    Q2dHelper();
    ~Q2dHelper();

    void paint(QPainter *painter, QPaintEvent *event, int elapsed);

    void paint_polygon(const std::vector<clib::v2> &v, bool statics);

private:
    static QPointF trans(const clib::v2 &v);
    static QSizeF mid;
    static bool mid_width;

private:
    QPainter *painter{ nullptr };
    QPaintEvent *event{ nullptr };
    QBrush background{ QColor(Qt::black) };
    QPen static_border{ QColor(230, 230, 230) };
    clib::c2d_world world;
};

