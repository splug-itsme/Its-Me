#include "editdig.hpp"
#include <qgraphicsview.h>
#include <QDirIterator>
#include <iostream>
#include<qstring.h>
#include<stdlib.h>
#include<qscrollbar.h>

editDig::editDig(QWidget * parent) : QWidget(parent) {
	ui.setupUi(this);
	dw = new QDesktopWidget();
	width = dw->geometry().width() / 2;
	height = dw->geometry().height() / 2;

	ui.listWidget->setViewMode(QListWidget::IconMode);
	ui.listWidget->setIconSize(QSize(200, 200));
	ui.listWidget->setResizeMode(QListWidget::Adjust);
	ui.listWidget->setSelectionMode(QListWidget::MultiSelection);
	}

editDig::~editDig() {

}
void editDig::init(cv::Mat bg, Person per,cv::Mat AImg)
{
	for (int i = 0; i < 32; i++)
		click[i] = false;

	backG = bg;
	editDig::AImg = AImg;
	background = Mat2QImage(bg);
	ui.label->setPixmap(QPixmap::fromImage(background).scaled(ui.label->size()));
	QListWidgetItem *list;
	editDig::per = per;
	for (int k = 0; k < per.size(); k++) {
		QImage qi= Mat2QImage(per.get_Frame(k));
		list = new QListWidgetItem(QPixmap::fromImage(qi),NULL);
		list->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
		ui.listWidget->insertItem(k,list);
	}


	this->show();
}

void editDig::saveImg() {
	QString name = QFileDialog::getSaveFileName(this, "save image", "untitle.png", "Images(*.png *.xpm *.jpg)");
	ui.label->pixmap()->toImage().save(name);
}
void editDig::checkItems(QListWidgetItem *item)
{

	int n = ui.listWidget->row(item);
	if (click[n] == false)
		click[n] = true;
	else
		click[n] = false;

	std::cout << n;
	int c;
	cv::Mat backG2;
	backG2 = backG;
	for (int j = 0; j < per.size(); j++)
	{	
		if (click[j] == true)
		{
			group = per.get_Group(j);

			for (int i = 0; i < group.size(); i++) // 그룹의 모든 이미지 집어 넣기
				backG2 = add_object(backG2, group[i].frame, cv::Point(group[i].vec.x + group[i].vec.w / 2, group[i].vec.y + group[i].vec.h / 2));
		}

		
	}
	float cutRate = 0.95; // 잘라낼 테두리 비율

	QImage mid = Mat2QImage(backG2(cv::Rect(backG2.cols * (1 - cutRate) / 2, backG2.rows * (1 - cutRate) / 2, backG2.cols * cutRate, backG2.rows * cutRate)));
	ui.label->setPixmap(QPixmap::fromImage(mid).scaled(ui.label->size()));
}

QImage  editDig::Mat2QImage(cv::Mat const& src)
{
	cv::Mat temp; // make the same cv::Mat
	cvtColor(src, temp, CV_BGR2RGB); // cvtColor Makes a copt, that what i need
	QImage dest((const uchar *)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
	dest.bits(); // enforce deep copy, see documentation 
				 // of QImage::QImage ( const uchar * data, int width, int height, Format format )
	return dest;
}

cv::Mat editDig::add_object(cv::Mat &background, cv::Mat &object, cv::Point center) {
	// center is object's center location
	cv::Mat src_mask = 255 * cv::Mat::ones(object.rows, object.cols, object.depth());
	cv::Mat result;
	seamlessClone(object, background, src_mask, center, result, cv::NORMAL_CLONE);
	return result;
}