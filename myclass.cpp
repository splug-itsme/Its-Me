#include "myclass.h"
#include "editdig.hpp"
#include<qmovie.h>
#include <thread>
#include<stdio.h>

using namespace cv;
using namespace std;



using namespace cv;
using namespace std;


MyClass::MyClass(QWidget *parent)
	: QMainWindow(parent)
{
	
	QDesktopWidget * dw = new QDesktopWidget();
	ui.setupUi(this);
	vf = new QCameraViewfinder(ui.verticalLayoutWidget);
	ui.verticalLayout->addWidget(vf);
	this->resize(dw->geometry().width(), dw->geometry().height());
	/*
	foreach(QCameraInfo info, QCameraInfo::availableCameras())
	{
		cam = new QCamera(info);
	}
	cam->setViewfinder(vf);
	vf->resize(dw->geometry().width(), dw->geometry().height());
	ui.verticalLayoutWidget->resize(dw->geometry().width(), dw->geometry().height());
<<<<<<< HEAD

=======
>>>>>>> 2ea6785f2c38dcfd06dd77455405a2a9e79e8fa4
	cam->setCaptureMode(QCamera::CaptureVideo);
	cam->start();
	*/

}

MyClass::~MyClass()
{

}

void MyClass::newDig() {


	if (ui.strBtn->text() == "start")
	{
		ui.strBtn->setText("save");

		VideoCapture capture(0);

	//	cam->stop();

		VideoWriter outputVideo;
		Mat frame;
		//capture >> frame;
		
		Size *s = new Size((int)frame.cols, (int)frame.rows);

		outputVideo.open("video.avi", -1,15, *s, true);

		/*if (!outputVideo.isOpened())
		{
			cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;
			return ;
		}*/

		//outputVideo.open("video.avi", -1,15, *s, true);

		if (!outputVideo.isOpened())
		{
			cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;
			return ;
		}


		if (!capture.isOpened())
		{
			cout << "웹캠을 열수 없습니다." << endl;
			return ;
		}

		//캡처 영상을 640x480으로 지정  
		capture.set(CAP_PROP_FRAME_WIDTH, 640);
		capture.set(CAP_PROP_FRAME_HEIGHT, 480);

		namedWindow("Cam", 1);
		int i = 0;

		int fps=30;
		//fps = capture.get(CV_CAP_PROP_FPS);
		//backGround bg;


	

		while(1)
		{
			//웹캡으로부터 한 프레임을 읽어옴  
			

			i++;
			if (!capture.read(frame))
				return;		
		
			outputVideo << frame;
		
			//화면에 영상을 보여줌
			imshow("Cam", frame);
			
			//동영상 5초 저장이 끝나면 종료.
			if ((fps * 5) == i)
			{
				cvDestroyWindow("Cam");
				break;
			}
			//ESC키 누르면 종료
			if (waitKey(1) == 27)
			{
				cvDestroyWindow("Cam");
				break;
			}
		}

		outputVideo.release();

		QMessageBox msgbox;
		msgbox.setText("aaaaaaaaaaaaaaaaaaaaaaaaaaa");
		msgbox.setWindowTitle("making...");
		msgbox.show();
		backGround bg = backGround("video.avi");
		back = bg.bg;
		per = bg.person;
		msgbox.setWindowTitle("complete");
		msgbox.setText("complete making backGround.!!");
		msgbox.exec();
		editDig *edit = new editDig();
		edit->init(back, per,bg.AImg);
		ui.editBtn->setEnabled(true);

		


		///////////////////////////////////////////여기에  opencv 캠 

	}
	else
	{

		QImage img;
		img.load("Blue.jpg");
		QPixmap *buf = new QPixmap();
		*buf = QPixmap::fromImage(img);

		QString name = QFileDialog::getSaveFileName(this, "save image", "untitle.png", "Images(*.png *.xpm *.jpg)");
		img.save(name);
	
	}
	 


}
void MyClass::newEdit() {

	

	
}
void MyClass::reStart() {

	ui.strBtn->setText("start");

}

void MyClass::processImage(QMediaRecorder::State *state)
{




	// sometime later, or on another press

	//shotDig mDig = new shotDig();

}


