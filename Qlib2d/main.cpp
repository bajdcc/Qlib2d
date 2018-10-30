#include "stdafx.h"
#include "qlib2d.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qlib2d w;
    w.show();
    return a.exec();
}
