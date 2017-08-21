#include "editdig.hpp"
#include <qgraphicsview.h>
#include <QDirIterator>
#include <iostream>
#include<qstring.h>
#include<stdlib.h>

editDig::editDig(QWidget * parent) : QWidget(parent) {
	ui.setupUi(this);
	dw = new QDesktopWidget();
	width = dw->geometry().width() / 2;
	height = dw->geometry().height() / 2;

	ui.listWidget->setViewMode(QListWidget::IconMode);
	ui.listWidget->setIconSize(QSize(200, 200));
	ui.listWidget->setResizeMode(QListWidget::Adjust);
	ui.listWidget->setSelectionMode(QListWidget::MultiSelection);

	
	//addImgItem();
	
//	this->show();
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
	ui.label->setPixmap(QPixmap::fromImage(background));
	QListWidgetItem *list;
	editDig::per = per;
	for (int k = 0; k < per.size(); k++) {
		QImage qi= Mat2QImage(per.get_Frame(k));
		list = new QListWidgetItem(QPixmap::fromImage(qi),NULL);
		list->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
		ui.listWidget->addItem(list);
	}


	this->show();
}
//void editDig::changeBack(cv::Mat) {}
void editDig::addImgItem() {
	
	
	QStringList strFilters;

	strFilters += "*.bmp";
	strFilters += "*.jpg";
	strFilters += "*.png";

	QString strDir("imgData");

	QDirIterator iterDir(strDir, strFilters, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
	QListWidgetItem *list;
	while (iterDir.hasNext())
	{
		iterDir.next();
		list = new QListWidgetItem(QPixmap(iterDir.filePath()), NULL);
		list->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
		ui.listWidget->addItem(list);
	}
}
void editDig::saveImg() {

	QString name = QFileDialog::getSaveFileName(this, "save image", "untitle.png", "Images(*.png *.xpm *.jpg)");
	ui.label->pixmap()->toImage().save(name);
}
void editDig::checkItems(QListWidgetItem *item)
{

	int n = ui.listWidget->row(item);
	//	ui.label->setText(""+n);
	if (click[n] == false)
		click[n] = true;
	else
		click[n] = false;

	std::cout << n;
	int c;
	Mat backG2;
	backG2 = backG;
	for (int j = 0; j < 4; j++)
	{	
		if (click[j] == true)
		{
			group = per.get_Group(j);

			for (int i = 0; i < group.size(); i++) // 그룹의 모든 이미지 집어 넣기
				backG2 = add_object(backG2, group[i].frame, Point(group[i].vec.x + group[i].vec.w / 2, group[i].vec.y + group[i].vec.h / 2));
		}

		
	}
	QImage mid = Mat2QImage(backG2);
	ui.label->setPixmap(QPixmap::fromImage(mid));
	//imwrite("result.bmp", backG);
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

Mat editDig::add_object(Mat &background, Mat &object, Point center) {
	// center is object's center location
	Mat src_mask = 255 * Mat::ones(object.rows, object.cols, object.depth());
	Mat result;
	seamlessClone(object, background, src_mask, center, result, NORMAL_CLONE);
	return result;
}