#include "stdafx.h"
#include "qlib2d.h"

Qlib2d::Qlib2d(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void Qlib2d::execLispCommand()
{
    auto code = ui.console->text();
    ui.console->setText("");
    if (code.isEmpty())
        return;
    try {
        parser = std::make_unique<clib::cparser>(code.toStdString());
        auto node = parser->parse();
        vm.prepare(node);
    }
    catch (const std::exception &e) {
        ui.output->setText("Error");
        parser.reset(nullptr);
        return;
    }
    vm.save();
    try {
        auto val = vm.run(INT32_MAX);
        if (val != nullptr) {
            std::stringstream ss;
            clib::cvm::print(val, ss);
            ui.output->setText(QString::fromStdString(ss.str()));
        }
        vm.gc();
        parser.reset(nullptr);
    }
    catch (const std::exception &e) {
        ui.output->setText("Error");
        vm.restore();
        vm.gc();
        parser.reset(nullptr);
    }
}
