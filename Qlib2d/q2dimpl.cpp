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

void Q2dImpl::keyPressEvent(QKeyEvent * event)
{
    auto key = event->key();
    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        helper.get_world().scene(key - '0');
    }
    switch (event->key()) {
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_Space:
        helper.get_world().toggle_pause();
        break;
    case Qt::Key_W:
        helper.get_world().move(clib::v2(0, 0.02));
        break;
    case Qt::Key_A:
        helper.get_world().move(clib::v2(-0.02, 0));
        break;
    case Qt::Key_S:
        helper.get_world().move(clib::v2(0, -0.02));
        break;
    case Qt::Key_D:
        helper.get_world().move(clib::v2(0.02, 0));
        break;
    case Qt::Key_Q:
        helper.get_world().rotate(0.02);
        break;
    case Qt::Key_E:
        helper.get_world().rotate(-0.02);
        break;
    case Qt::Key_G:
        helper.get_world().toggle_gravity();
        break;
    }
}
