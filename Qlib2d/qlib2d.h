#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qlib2d.h"
#include "cvm.h"
#include "cparser.h"

class Qlib2d : public QMainWindow
{
    Q_OBJECT

public:
    Qlib2d(QWidget *parent = Q_NULLPTR);

signals:
    void send_command(QString &);

private:
    Ui::Qlib2dClass ui;

private slots:
    void execLispCommand();
    void escape();
    void output(QString, int);
};
