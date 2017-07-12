#include "shotdig.hpp"

shotDig::shotDig(QWidget * parent) : QWidget(parent) {
	ui.setupUi(this);
	dw = new QDesktopWidget();
	ui.setupUi(this);
	QImage img;
	img.load("Blue.jpg");
	QPixmap *buf = new QPixmap();
	*buf = QPixmap::fromImage(img);
	ui.imgLbl->setPixmap(*buf);
	ui.imgLbl->resize(dw->geometry().width(), dw->geometry().height());
	this->show();
}

shotDig::~shotDig() {
	
}
