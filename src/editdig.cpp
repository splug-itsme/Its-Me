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

	ui.listWidget->setViewMode(QListWidget::IconMode);
	ui.listWidget->setIconSize(QSize(200, 200));
	ui.listWidget->setResizeMode(QListWidget::Adjust);
	ui.listWidget->setSelectionMode(QListWidget::MultiSelection);
	//ui widget setting	
}

editDig::~editDig() {

}
//init function
void editDig::init(cv::Mat cut_bg, PersonSet per,cv::Mat big_bg)
{
	for (int i = 0; i < 32; i++)	//init click 
		click[i] = false;			//click means person click

	backG = cut_bg;	//cutted background 
	sub_Background = Mat2QImage(big_bg);
	ui.label->setPixmap(QPixmap::fromImage(sub_Background).scaled(ui.label->size()));
	QListWidgetItem *list;//clickable items 
	editDig::per = per;		//parameter person
	for (int k = 0; k < per.size(); k++) {
		QImage qi= Mat2QImage(per.get_Frame(k));//save item as Qimage type
		list = new QListWidgetItem(QPixmap::fromImage(qi),NULL);//add item to list 
		list->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
		ui.listWidget->insertItem(k,list);//add to widget
	}


	this->show();
}

//save image
void editDig::saveImg() {
	QString name = QFileDialog::getSaveFileName(this, "save image", "untitle.png", "Images(*.png *.xpm *.jpg)");//result image save 
	ui.label->pixmap()->toImage().save(name);
}

// Object restoration function
void editDig::checkItems(QListWidgetItem *item)
{

	int n = ui.listWidget->row(item);

	//When the object is clicked
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
		//if click is true, clicked object restoration
		if (click[j] == true)
		{
			group = per.get_Group(j);
			//// Put all images in a group
			for (int i = 0; i < group.size(); i++) 
				backG2 = add_object(backG2, group[i].frame, cv::Point(group[i].vec.x + group[i].vec.w / 2, group[i].vec.y + group[i].vec.h / 2));
		}

		
	}
	float cutRate = 0.95; // Border Ratio to crop

	QImage mid = Mat2QImage(backG2(cv::Rect(backG2.cols * (1 - cutRate) / 2, backG2.rows * (1 - cutRate) / 2, backG2.cols * cutRate, backG2.rows * cutRate)));//apply cutrate 
	ui.label->setPixmap(QPixmap::fromImage(mid).scaled(ui.label->size()));//show image to widget
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
//add person in background
cv::Mat editDig::add_object(cv::Mat &sub_Background, cv::Mat &object, cv::Point center) {
	// center is object's center location
	cv::Mat src_mask = 255 * cv::Mat::ones(object.rows, object.cols, object.depth());
	cv::Mat result;
	seamlessClone(object, sub_Background, src_mask, center, result, cv::NORMAL_CLONE);
	return result;
}