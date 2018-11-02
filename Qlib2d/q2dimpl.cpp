#include "stdafx.h"

#include "q2dimpl.h"


Q2dImpl::Q2dImpl(QWidget *parent) : QOpenGLWidget(parent)
{
    auto timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(PAINT_SPAN);
}


Q2dImpl::~Q2dImpl()
{
}

void Q2dImpl::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}

void Q2dImpl::paintEvent(QPaintEvent * event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    helper.paint(&painter, event, elapsed);
    painter.end();
}
