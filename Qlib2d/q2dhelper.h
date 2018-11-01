#pragma once

#include "c2dworld.h"

class Q2dHelper
{
public:
    Q2dHelper();
    ~Q2dHelper();

    void paint(QPainter *painter, QPaintEvent *event, int elapsed);

private:
    QBrush background{ QColor(Qt::black) };
    clib::c2d_world world;
};

