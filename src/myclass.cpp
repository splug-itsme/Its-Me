#include "myclass.h"
#include "editdig.hpp"
#include<qmovie.h>
#include <thread>
#include<stdio.h>

using namespace cv;
using namespace std;


MyClass::MyClass(QWidget *parent)
	: QMainWindow(parent)
{

	QDesktopWidget * dw = new QDesktopWidget();
	ui.setupUi(this);
	vf = new QCameraViewfinder(ui.verticalLayoutWidget);//camera layout setting
	ui.verticalLayout->addWidget(vf);
	
		/* 
		//using webcam
		foreach(QCameraInfo info, QCameraInfo::availableCameras())
		{
			cam = new QCamera(info);//웹캡의 설정을 가져온다
		}
		cam->setViewfinder(vf);
		vf->resize(dw->geometry().width(), dw->geometry().height());
		ui.verticalLayoutWidget->resize(dw->geometry().width(), dw->geometry().height());//위젯의 크기를 카메라 영상 크기로 설정해준다.
		cam->setCaptureMode(QCamera::CaptureVideo);
		cam->start();
		*/

}

MyClass::~MyClass()
{

}

void MyClass::newDig() {
	//This part is executed when the first button is pressed.

	if (ui.strBtn->text() == "start")//If the state of the button is 'start'
	{
		ui.strBtn->setText("save");//Set the state of the button to 'save'.


		QFileDialog fileDialog(this, tr("Open File"), tr("C:"), tr("movie (*.mp4 *.avi )"));//Create video file selection dialog 
		QStringList fileNames;
		if (fileDialog.exec()) {
			fileNames = fileDialog.selectedFiles();//save filenames to list 
		}

		QString selectedFile;
		for (int nIndex = 0; nIndex < fileNames.size(); nIndex++) {
			selectedFile.append(fileNames.at(nIndex).toLocal8Bit().constData());//append filename to absolute path.
		}
		/*
		//using webcam

		cam->stop();
		capture >> frame;
		*/
		VideoCapture capture(selectedFile.toStdString());//Brings frames of selected video
		VideoWriter outputVideo;
		Mat frame;
	
		Size *s = new Size((int)frame.cols, (int)frame.rows);// frame size

		outputVideo.open("video.avi", 0, 30, *s, true);		// open video.avi at 30 frames per second

	/* using webcam
	if (!outputVideo.isOpened())
		{
			cout << "Error occurred during initialization to save movie" << endl;
			return;
		}
*/
		//can not open video
		if (!capture.isOpened())
		{
			cout << "can not open video" << endl;
			return;
		}

		//Specify captured image as 640x480  
		capture.set(CAP_PROP_FRAME_WIDTH, 640);
		capture.set(CAP_PROP_FRAME_HEIGHT, 480);

		namedWindow("Cam", 1);
		int i = 0;

		int fps = 30;	//fps

		while (1)
		{
			i++;
			if (!capture.read(frame))	//no frame to read 
				break;
			
			outputVideo << frame;	// read 1 frame from outputVideo

	
			imshow("Cam", frame);	// show video on the screen

			//Exit after (fps * 8)frame of video saving.
			if ((fps * 8) == i)
			{
				cvDestroyWindow("Cam");
				break;
			}
	
			if (waitKey(1) == 27)  //Input ESC key
			{
				cvDestroyWindow("Cam"); //finish Cam
				break;
			}
		}

		outputVideo.release();

		QMessageBox msgbox;		//message box
		msgbox.setText("aaaaaaaaaaaaaaaaaaaaaaaaaaa");	//message box contents
		msgbox.setWindowTitle("making...");	//message box title
		msgbox.show();	//message box show
		sub_Background bg = sub_Background("video.avi");	//call sub_Background
		back = bg.bg;
		per = bg.person_set;
		msgbox.setWindowTitle("complete");		//message box title
		msgbox.setText("complete making Background.!!"); //message box contents
		msgbox.exec();	//exec message box
		editDig *edit = new editDig();	//call editDig
		edit->init(back, per, bg.big_backImg); //init editDig
		ui.editBtn->setEnabled(true);

	}


}
void MyClass::reStart() {

	ui.strBtn->setText("start");

}
