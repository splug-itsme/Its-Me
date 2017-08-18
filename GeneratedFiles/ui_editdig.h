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
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_editDig
{
public:
    QListWidget *listWidget;
    QFrame *frame;
    QLabel *label;
    QPushButton *canBtn;
    QPushButton *saveBtn;

    void setupUi(QWidget *editDig)
    {
        if (editDig->objectName().isEmpty())
            editDig->setObjectName(QStringLiteral("editDig"));
        editDig->resize(1590, 800);
        listWidget = new QListWidget(editDig);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(1350, 10, 221, 721));
        listWidget->setFlow(QListView::TopToBottom);
        listWidget->setViewMode(QListView::IconMode);
        frame = new QFrame(editDig);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(20, 10, 1301, 771));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 1301, 771));
        canBtn = new QPushButton(editDig);
        canBtn->setObjectName(QStringLiteral("canBtn"));
        canBtn->setGeometry(QRect(1350, 740, 111, 31));
        saveBtn = new QPushButton(editDig);
        saveBtn->setObjectName(QStringLiteral("saveBtn"));
        saveBtn->setGeometry(QRect(1470, 740, 101, 31));

        retranslateUi(editDig);
        QObject::connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), editDig, SLOT(checkItems(QListWidgetItem*)));
        QObject::connect(canBtn, SIGNAL(clicked()), editDig, SLOT(close()));
        QObject::connect(saveBtn, SIGNAL(clicked()), editDig, SLOT(saveImg()));

        QMetaObject::connectSlotsByName(editDig);
    } // setupUi

    void retranslateUi(QWidget *editDig)
    {
        editDig->setWindowTitle(QApplication::translate("editDig", "editDig", 0));
        label->setText(QString());
        canBtn->setText(QApplication::translate("editDig", "cancel", 0));
        saveBtn->setText(QApplication::translate("editDig", "save", 0));
    } // retranslateUi

};

namespace Ui {
    class editDig: public Ui_editDig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITDIG_H
