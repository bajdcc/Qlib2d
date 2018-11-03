#pragma once

#include <QtWidgets/QOpenGLWidget>
#include "q2dhelper.h"

class Q2dImpl : public QOpenGLWidget
{
    Q_OBJECT

public:
    Q2dImpl(QWidget *parent = nullptr);
    ~Q2dImpl();

    Q2dHelper * get_helper();

signals:
    void escape();

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;

private:
    int elapsed{ 0 };
    Q2dHelper helper;
};

