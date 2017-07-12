#include "myclass.h"
#include "shotdig.hpp"

MyClass::MyClass(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	vf = new QCameraViewfinder(ui.verticalLayoutWidget);
	ui.verticalLayout->addWidget(vf);

	foreach(QCameraInfo info, QCameraInfo::availableCameras())
	{
		cam = new QCamera(info);
	}
	cam->setViewfinder(vf);
	cic = new QCameraImageCapture(cam);
	connect(cic, SIGNAL(imageCaptured(int, QImage)), this, SLOT(processImage(int, QImage)));
	cam->setCaptureMode(QCamera::CaptureStillImage);
	cam->start();
}

MyClass::~MyClass()
{

}


void MyClass::newDig() {
	
	shotDig mDig = new shotDig();


}


void MyClass::processImage(int i, QImage img)
{
	QString name = QFileDialog::getSaveFileName();
	img.save(name);
	//cam->stop();
}



QImage  MyClass::Mat2QImage(cv::Mat const& src)
{
	cv::Mat temp; // make the same cv::Mat
	cvtColor(src, temp, CV_BGR2RGB); // cvtColor Makes a copt, that what i need
	QImage dest((const uchar *)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
	dest.bits(); // enforce deep copy, see documentation 
				 // of QImage::QImage ( const uchar * data, int width, int height, Format format )
	return dest;
}