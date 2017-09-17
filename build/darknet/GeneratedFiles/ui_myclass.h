/********************************************************************************
** Form generated from reading UI file 'myclass.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYCLASS_H
#define UI_MYCLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyClassClass
{
public:
    QWidget *centralWidget;
    QPushButton *strBtn;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *editBtn;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MyClassClass)
    {
        if (MyClassClass->objectName().isEmpty())
            MyClassClass->setObjectName(QStringLiteral("MyClassClass"));
        MyClassClass->setEnabled(true);
        MyClassClass->resize(1115, 756);
        centralWidget = new QWidget(MyClassClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        strBtn = new QPushButton(centralWidget);
        strBtn->setObjectName(QStringLiteral("strBtn"));
        strBtn->setGeometry(QRect(20, 20, 70, 50));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 1091, 681));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        editBtn = new QPushButton(centralWidget);
        editBtn->setObjectName(QStringLiteral("editBtn"));
        editBtn->setEnabled(false);
        editBtn->setGeometry(QRect(100, 20, 70, 50));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(180, 20, 70, 50));
        MyClassClass->setCentralWidget(centralWidget);
        verticalLayoutWidget->raise();
        strBtn->raise();
        editBtn->raise();
        pushButton->raise();
        menuBar = new QMenuBar(MyClassClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1115, 21));
        MyClassClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MyClassClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MyClassClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MyClassClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MyClassClass->setStatusBar(statusBar);

        retranslateUi(MyClassClass);
        QObject::connect(strBtn, SIGNAL(clicked()), MyClassClass, SLOT(newDig()));
        QObject::connect(editBtn, SIGNAL(clicked()), MyClassClass, SLOT(newEdit()));
        QObject::connect(pushButton, SIGNAL(clicked()), MyClassClass, SLOT(reStart()));

        QMetaObject::connectSlotsByName(MyClassClass);
    } // setupUi

    void retranslateUi(QMainWindow *MyClassClass)
    {
        MyClassClass->setWindowTitle(QApplication::translate("MyClassClass", "MyClass", 0));
        strBtn->setText(QApplication::translate("MyClassClass", "start", 0));
        editBtn->setText(QApplication::translate("MyClassClass", "edit", 0));
        pushButton->setText(QApplication::translate("MyClassClass", "restart", 0));
    } // retranslateUi

};

namespace Ui {
    class MyClassClass: public Ui_MyClassClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYCLASS_H
