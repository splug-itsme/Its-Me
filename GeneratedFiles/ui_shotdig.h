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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_shotDig
{
public:
    QLabel *imgLbl;

    void setupUi(QWidget *shotDig)
    {
        if (shotDig->objectName().isEmpty())
            shotDig->setObjectName(QStringLiteral("shotDig"));
        shotDig->resize(400, 300);
        imgLbl = new QLabel(shotDig);
        imgLbl->setObjectName(QStringLiteral("imgLbl"));
        imgLbl->setGeometry(QRect(150, 170, 64, 15));

        retranslateUi(shotDig);

        QMetaObject::connectSlotsByName(shotDig);
    } // setupUi

    void retranslateUi(QWidget *shotDig)
    {
        shotDig->setWindowTitle(QApplication::translate("shotDig", "shotDig", 0));
        imgLbl->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class shotDig: public Ui_shotDig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOTDIG_H
