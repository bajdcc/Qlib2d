#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qlib2d.h"

class Qlib2d : public QMainWindow
{
    Q_OBJECT

public:
    Qlib2d(QWidget *parent = Q_NULLPTR);

private:
    Ui::Qlib2dClass ui;
};
