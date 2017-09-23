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
	vf = new QCameraViewfinder(ui.verticalLayoutWidget);
	ui.verticalLayout->addWidget(vf);
	
		/* 웹캡을 사용해서 프로젝트를 실행할때 들어가는 코드 
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
	//첫번째 버튼을 눌럿을때 이 부분이 실행된다.

	if (ui.strBtn->text() == "start")//버튼의 상태가 start 일 경우 
	{
		ui.strBtn->setText("save");//버튼의 상태를 save로  설정해준다

		QFileDialog fileDialog(this, tr("Open File"), tr("C:"), tr("movie (*.mp4 *.avi )"));//동영상 파일 선택 다이얼로그 생성 
		QStringList fileNames;
		if (fileDialog.exec()) {
			fileNames = fileDialog.selectedFiles();
		}

		QString selectedFile;
		for (int nIndex = 0; nIndex < fileNames.size(); nIndex++) {
			selectedFile.append(fileNames.at(nIndex).toLocal8Bit().constData());
		}
		/*웹캠 사용사 코드
			cam->stop();
		capture >> frame;
		*/
		VideoCapture capture(selectedFile.toStdString());//선택된 동영상의 프레임을 가져온다
		VideoWriter outputVideo;
		Mat frame;
	
		Size *s = new Size((int)frame.cols, (int)frame.rows);

		outputVideo.open("video.avi", 0, 30, *s, true);

	/*	if (!outputVideo.isOpened())
		{
			cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;
			return;
		}
*/

		if (!capture.isOpened())
		{
			cout << "웹캠을 열수 없습니다." << endl;
			return;
		}

		//캡처 영상을 640x480으로 지정  
		capture.set(CAP_PROP_FRAME_WIDTH, 640);
		capture.set(CAP_PROP_FRAME_HEIGHT, 480);

		namedWindow("Cam", 1);
		int i = 0;

		int fps = 30;

		while (1)
		{
			//웹캡으로부터 한 프레임을 읽어옴  


			i++;
			if (!capture.read(frame))//읽어올 프레임이 없다면 
				break;//멈춤

			outputVideo << frame;

			//화면에 영상을 보여줌
			imshow("Cam", frame);

			//동영상 5초 저장이 끝나면 종료.
			if ((fps * 8) == i)
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
		edit->init(back, per, bg.AImg);
		ui.editBtn->setEnabled(true);

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
void MyClass::reStart() {

	ui.strBtn->setText("start");

}
