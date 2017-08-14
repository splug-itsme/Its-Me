/********************************************************************************
** Form generated from reading UI file 'editdig.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITDIG_H
#define UI_EDITDIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_editDig
{
public:
    QListWidget *listWidget;
    QLabel *label;

    void setupUi(QWidget *editDig)
    {
        if (editDig->objectName().isEmpty())
            editDig->setObjectName(QStringLiteral("editDig"));
        editDig->resize(928, 656);
        listWidget = new QListWidget(editDig);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(685, 10, 241, 641));
        label = new QLabel(editDig);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 611, 591));

        retranslateUi(editDig);
        QObject::connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), editDig, SLOT(checkItems(QListWidgetItem*)));

        QMetaObject::connectSlotsByName(editDig);
    } // setupUi

    void retranslateUi(QWidget *editDig)
    {
        editDig->setWindowTitle(QApplication::translate("editDig", "editDig", 0));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class editDig: public Ui_editDig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITDIG_H
