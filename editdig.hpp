
#pragma once
#include "ui_editdig.h"
#include <QWidget>
#include <qdesktopwidget.h>
#include <qlabel.h>
#include <QTableWidget>
#include <qdir.h>
#include <QFileDialog>
#include"Person.h"
#include"editdig.hpp"

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
	vector<obj_t> group;
	Mat AImg, backG;
	void init(cv::Mat bg, Person per, cv::Mat AImg);
	Mat editDig::add_object(Mat &background, Mat &object, Point center);
	int width, height;
	void addImgItem();
	QImage Mat2QImage(cv::Mat const& src);
	public slots:
	void saveImg();
	void checkItems(QListWidgetItem*);
private:
	Ui::editDig ui;
};
