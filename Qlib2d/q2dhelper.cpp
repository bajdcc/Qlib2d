#include "stdafx.h"
#include "q2dhelper.h"


Q2dHelper::Q2dHelper()
{
    world.make_bound();
}


Q2dHelper::~Q2dHelper()
{
}

void Q2dHelper::paint(QPainter * painter, QPaintEvent * event, int elapsed)
{
    painter->fillRect(event->rect(), background);
    world.step();
}
