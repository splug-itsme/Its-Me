/********************************************************************************
** Form generated from reading UI file 'shotdig.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOTDIG_H
#define UI_SHOTDIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_shotDig
{
public:
    QLabel *imgLbl;
    QPushButton *saveBtn;
    QPushButton *edit;
    QPushButton *canBtn;

    void setupUi(QWidget *shotDig)
    {
        if (shotDig->objectName().isEmpty())
            shotDig->setObjectName(QStringLiteral("shotDig"));
        shotDig->resize(400, 300);
        imgLbl = new QLabel(shotDig);
        imgLbl->setObjectName(QStringLiteral("imgLbl"));
        imgLbl->setGeometry(QRect(150, 170, 64, 15));
        saveBtn = new QPushButton(shotDig);
        saveBtn->setObjectName(QStringLiteral("saveBtn"));
        saveBtn->setGeometry(QRect(10, 10, 80, 30));
        edit = new QPushButton(shotDig);
        edit->setObjectName(QStringLiteral("edit"));
        edit->setGeometry(QRect(100, 10, 80, 30));
        canBtn = new QPushButton(shotDig);
        canBtn->setObjectName(QStringLiteral("canBtn"));
        canBtn->setGeometry(QRect(190, 10, 80, 30));

        retranslateUi(shotDig);
        QObject::connect(saveBtn, SIGNAL(clicked()), shotDig, SLOT(saveImg()));
        QObject::connect(edit, SIGNAL(clicked()), shotDig, SLOT(editImg()));
        QObject::connect(canBtn, SIGNAL(clicked()), shotDig, SLOT(close()));

        QMetaObject::connectSlotsByName(shotDig);
    } // setupUi

    void retranslateUi(QWidget *shotDig)
    {
        shotDig->setWindowTitle(QApplication::translate("shotDig", "shotDig", 0));
        imgLbl->setText(QString());
        saveBtn->setText(QApplication::translate("shotDig", "save", 0));
        edit->setText(QApplication::translate("shotDig", "edit", 0));
        canBtn->setText(QApplication::translate("shotDig", "cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class shotDig: public Ui_shotDig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOTDIG_H
