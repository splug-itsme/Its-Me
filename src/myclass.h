#ifndef MYCLASS_H
#define MYCLASS_H

#include <QtWidgets/QMainWindow>
#include "opencv2/opencv.hpp"  
#include "ui_myclass.h"
#include <qcamera.h>
#include <QCameraImageCapture>
#include <QMessageBox>
#include <QCameraInfo>
#include <QLayout>
#include <QCameraViewfinder>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <qvideoprobe.h>
#include <QMediaRecorder>
#include <time.h> 
#include <qstring.h>
#include "qtextcodec.h"
#include <qfiledialog.h>
#include "backGround.h"


class MyClass : public QMainWindow
{
	Q_OBJECT

public:
	MyClass(QWidget *parent = 0);
	~MyClass();
	
	QCameraViewfinder * vf;
	Person per;
	cv::Mat back;

	QCamera * cam;

private:
	Ui::MyClassClass ui;
	public slots:
	void processImage(QMediaRecorder::State *state);
	void newDig();
	void reStart();
};

#endif // MYCLASS_H
