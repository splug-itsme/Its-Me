
#pragma once
#include "ui_editdig.h"
#include <QWidget>
#include <qdesktopwidget.h>
#include <qlabel.h>
#include <QTableWidget>
#include <qdir.h>
#include"Person.h"
#include"editdig.hpp"
#include <QFileDialog>
class editDig : public QWidget {
	Q_OBJECT

public:
	bool click[32];
	editDig(QWidget * parent = Q_NULLPTR);
	~editDig();
	QDesktopWidget *dw;
	QImage img;
	QImage background;
	Person per;
	std::vector<obj_t> group;
	cv::Mat AImg, backG;
	void init(cv::Mat bg, Person per, cv::Mat AImg);
	cv::Mat editDig::add_object(cv::Mat &background, cv::Mat &object, cv::Point center);
	int width, height;
	QImage Mat2QImage(cv::Mat const& src);
	public slots:
	void saveImg();
	void checkItems(QListWidgetItem*);
private:
	Ui::editDig ui;
};
