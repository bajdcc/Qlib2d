/********************************************************************************
** Form generated from reading UI file 'qlib2d.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QLIB2D_H
#define UI_QLIB2D_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "q2dimpl.h"

QT_BEGIN_NAMESPACE

class Ui_Qlib2dClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLineEdit *console;
    QTextEdit *output;
    Q2dImpl *opengl2d;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Qlib2dClass)
    {
        if (Qlib2dClass->objectName().isEmpty())
            Qlib2dClass->setObjectName(QStringLiteral("Qlib2dClass"));
        Qlib2dClass->resize(800, 600);
        centralWidget = new QWidget(Qlib2dClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        console = new QLineEdit(centralWidget);
        console->setObjectName(QStringLiteral("console"));
        console->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(console);

        output = new QTextEdit(centralWidget);
        output->setObjectName(QStringLiteral("output"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(output->sizePolicy().hasHeightForWidth());
        output->setSizePolicy(sizePolicy);
        output->setMaximumSize(QSize(16777215, 60));

        verticalLayout->addWidget(output);

        opengl2d = new Q2dImpl(centralWidget);
        opengl2d->setObjectName(QStringLiteral("opengl2d"));

        verticalLayout->addWidget(opengl2d);


        verticalLayout_2->addLayout(verticalLayout);

        Qlib2dClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Qlib2dClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 23));
        Qlib2dClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Qlib2dClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Qlib2dClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Qlib2dClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Qlib2dClass->setStatusBar(statusBar);

        retranslateUi(Qlib2dClass);
        QObject::connect(console, SIGNAL(returnPressed()), Qlib2dClass, SLOT(execLispCommand()));

        QMetaObject::connectSlotsByName(Qlib2dClass);
    } // setupUi

    void retranslateUi(QMainWindow *Qlib2dClass)
    {
        Qlib2dClass->setWindowTitle(QApplication::translate("Qlib2dClass", "Qlib2d", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Qlib2dClass: public Ui_Qlib2dClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QLIB2D_H
