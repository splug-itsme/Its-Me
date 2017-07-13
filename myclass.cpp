#include "myclass.h"
#include "shotdig.hpp"

MyClass::MyClass(QWidget *parent)
	: QMainWindow(parent)
{
	QDesktopWidget * dw = new QDesktopWidget();
	ui.setupUi(this);
	vf = new QCameraViewfinder(ui.verticalLayoutWidget);
	ui.verticalLayout->addWidget(vf);
	this->resize(dw->geometry().width(), dw->geometry().height());

	foreach(QCameraInfo info, QCameraInfo::availableCameras())
	{
		cam = new QCamera(info);
	}
	cam->setViewfinder(vf);
	vf->resize(dw->geometry().width(), dw->geometry().height());
	ui.verticalLayoutWidget->resize(dw->geometry().width(), dw->geometry().height());
	cic = new QCameraImageCapture(cam);
	connect(cic, SIGNAL(imageCaptured(int, QImage)), this, SLOT(processImage(int, QImage)));
	cam->setCaptureMode(QCamera::CaptureStillImage);
	cam->start();

}

MyClass::~MyClass()
{

}


void MyClass::newDig() {
	cic->capture();
	//shotDig mDig = new shotDig();


}


void MyClass::processImage(int i, QImage img)
{
	//QString name = QFileDialog::getSaveFileName();
	//img.save(name);
	cam->stop();

	QMediaRecorder * recorder = new QMediaRecorder(cam);

	cam->setCaptureMode(QCamera::CaptureVideo);
	cam->start();

	//on shutter button pressed
	recorder->record();
	recorder->setOutputLocation;

	// sometime later, or on another press
	recorder->stop();
	shotDig mDig = new shotDig();

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