#include "stdafx.h"
#include "qlib2d.h"

Qlib2d::Qlib2d(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(this, SIGNAL(send_command(QString &)), ui.opengl2d->get_helper(), SLOT(exec(QString &)));
    connect(ui.opengl2d, SIGNAL(escape()), this, SLOT(escape()));
    connect(ui.opengl2d->get_helper(), SIGNAL(output(QString &)), this, SLOT(output(QString &)));
}

void Qlib2d::execLispCommand()
{
    auto code = ui.console->text();
    if (code.isEmpty())
        return;
    if (code == tr("exit"))
    {
        close();
        return;
    }
    emit send_command(code);
    if (code.isEmpty())
        ui.console->setText(tr(""));
}

void Qlib2d::escape()
{
    close();
}

void Qlib2d::output(QString & str)
{
    ui.output->setText(str);
}
