#include "stdafx.h"
#include "qlib2d.h"

Qlib2d::Qlib2d(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(this, SIGNAL(send_command(QString &)), ui.opengl2d->get_helper(), SLOT(exec(QString &)));
    connect(ui.opengl2d, SIGNAL(escape()), this, SLOT(escape()));
    connect(ui.opengl2d->get_helper(), SIGNAL(output(QString &, int)), this, SLOT(output(QString &, int)));
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
    output((QString("Running lisp...\n%1").arg(code)), 0);
    emit send_command(code);
    if (code.isEmpty())
        output(tr(""), 1);
}

void Qlib2d::escape()
{
    close();
}

void Qlib2d::output(QString & str, int type)
{
    if (type == 0)
        ui.output->setText(str);
    else if (type == 1)
        ui.console->setText(str);
}
