#pragma once
#include <QWidget>
#include "ui_shotdig.h"
#include <qdesktopwidget.h>
class shotDig : public QWidget {
	Q_OBJECT

public:
	shotDig(QWidget * parent = Q_NULLPTR);
	~shotDig();
	QDesktopWidget *dw;

private:
	Ui::shotDig ui;
};
