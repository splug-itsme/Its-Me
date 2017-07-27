
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "opencv\cxcore.h"
#include "opencv\cvaux.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <thread>

#define DEGREE 10

using namespace cv;
using namespace std;

void changeGray(Mat &Img);
void cvDiff(Mat &image, Mat &image2, Mat &diff);
void copy(const Mat &Img, Mat &result, Mat &mask);
void checkDegree(vector <Mat> &frame, int start, int end);
int sampleVal(vector <int> &agrDegree); 

Mat des;

int main(int argc, char *argv[])
{
//	CvCapture *capture = cvCaptureFromFile("mog3.avi");
	VideoCapture capture("3.mp4");
	VideoWriter vw;
	vw = VideoWriter("AA.mp4", CV_FOURCC('D', 'I', 'V', 'X'), capture.get(CV_CAP_PROP_FPS), Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH), (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT)), true);

	if (!vw.isOpened())
	{
		cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;
		return 1;
	}
	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << "tt" << endl;
		exit(EXIT_FAILURE);
	}
	cvNamedWindow("Image");
	cvNamedWindow("Image2");
	cvNamedWindow("Image3");
	cvNamedWindow("tmpImage");
	cvNamedWindow("diffImage");
	cvNamedWindow("resultImage");
	cvNamedWindow("tmp2");
	
	//Mat t;
	//capture.read(t);
	//imwrite("test.bmp", t);
	//exit(0);
	vector<Mat> src;

	Mat Img = imread("result.bmp");
	Mat Img2 = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));
	Mat Img3 = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));
	Mat Img4 = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));

	Mat res = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));
	Mat diff = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));
	Mat tmp = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));
	Mat MOG = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));

	des = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));
	
	Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
	pMOG2 = createBackgroundSubtractorMOG2();
	int check = 0;
	vector <Mat> frame;

	while (capture.read(Img2))
	{
		check++;
		if (check == 0)
			continue;
		if (check % 5 != 0)
			continue;
		Mat result = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));
		
		absdiff(Img, Img2, diff);
		threshold(diff, tmp, 70 ,255, CV_THRESH_BINARY);
		
	
		//cvtColor(tmp, tmp, CV_BGR2GRAY);
		changeGray(tmp);
		 
		//Img2.copyTo(Img, tmp); // inputMat.copyTo(outputMat, maskMat);

		
		pMOG2->apply(tmp, diff);
		erode(diff, diff, Mat(), Point(-1, -1), 3);
		//blur(diff, diff, Size(5, 5)); // noise 제거
		diff = ~diff;
		Img2.copyTo(result, diff);
	//	tmp = tmp & diff;



		frame.push_back(result);
		
	
	/*	if (frame.size() == 10) {
			checkDegree(frame);
			src.push_back(Img3);
			frame.clear();
		}*/	
		
		imshow("diffImage", diff);
		imshow("tmpImage", tmp);
		imshow("resultImage", res);
		imshow("Image", Img);
		imshow("Image2", Img2);
		imshow("Image3", Img3);
		imshow("tmp2", result);
		
		if (cvWaitKey() == 27) continue; // break에서 바꿈 키입력받을때마다 프레임이동
	
	}
	//checkDegree(src);
	
	/*int size = frame[0].rows / 6;
	thread t1(&checkDegree, frame , 0, size);
	thread t2(&checkDegree, frame, size, size * 2);
	thread t3(&checkDegree, frame, size * 2, size * 3);
	thread t4(&checkDegree, frame, size * 3, size * 4);
	thread t5(&checkDegree, frame, size * 4, size * 5);
	thread t6(&checkDegree, frame, size * 5, size * 6);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();*/


	imwrite("result.bmp", des);
	capture.release();
	vw.release();


	cvDestroyWindow("tt");
	
	return 0;
}
void checkDegree(vector <Mat> &frame, int start, int end) {  // 단일 구간의 프레임들 , des는 구간의 대표값
	Vec3b* desMat = (Vec3b*)des.data;
	printf("%d %d\n", start, end);
 //	for (int i = 0; i < frame[0].rows; i++) {
	for (int i = start; i < end; i++) {
		for (int j = 0; j < frame[0].cols; j++) {
			vector <int> agrDegree(frame.size(), 0);
			for (int k = 0; k < frame.size(); k++) { 
				 for (int t = k+1; t < frame.size(); t++) {
						Vec3b* data = (Vec3b*) frame[k].data;
						Vec3b* data2 = (Vec3b*) frame[t].data;
						if (abs(data[i * frame[0].cols + j][0] - data2[i * frame[0].cols + j][0]) < 10 && abs(data[i * frame[0].cols + j][1] - data2[i * frame[0].cols + j][1]) < 10 && abs(data[i * frame[0].cols + j][2] - data2[i * frame[0].cols + j][2]) < 10) { // DEGREE보다 작으면 증가
							agrDegree[k]++;
							agrDegree[t]++;
						}
				 }
			}
			int ret = sampleVal(agrDegree);
			Vec3b* tmp = (Vec3b*)frame[ret].data;
			desMat[i * frame[0].cols + j] = tmp[i * frame[0].cols + j];
			//vw.write(des);
		}
		//printf("%d\n", i);
	}

	imwrite("Img.bmp", frame[0]);
	imwrite("Img2.bmp", frame[1]);
	imwrite("result.bmp", des);

}
int sampleVal(vector <int> &agrDegree)
{
	int ret = *max_element(agrDegree.begin(), agrDegree.end()); // 일치도 최대 구하기

	for (int i = 0; i < agrDegree.size(); i++) { // 이진검색하면 시간좀 줄일수 잇음
		if (agrDegree[i] == ret)
			return i;
	}
	return 0;
}

void cvDiff(Mat &image, Mat &image2,  Mat &diff) {
	int x, y;
	for (y = 0; y < image.rows; y++)
	{
		for (x = 0; x < image.cols; x++)
		{
			if (image.channels() == 1) // Gray 영상일 경우
			{
				diff.at<uchar>(y, x) = abs(image.at<uchar>(y, x) - image2.at<uchar>(y, x));
			}
			else if (image.channels() == 3) // RGB 영상일 경우
			{
				diff.at<Vec3b>(y, x)[0] = abs(image.at<Vec3b>(y, x)[0] - image2.at<Vec3b>(y, x)[0]);
				diff.at<Vec3b>(y, x)[1] = abs(image.at<Vec3b>(y, x)[1] - image2.at<Vec3b>(y, x)[1]);
				diff.at<Vec3b>(y, x)[2] = abs(image.at<Vec3b>(y, x)[2] - image2.at<Vec3b>(y, x)[2]);
			}
		}
	}
}


void changeGray(Mat &Img)
{
	Vec3b* data = (Vec3b*)Img.data;
	for (int i = 0; i < Img.rows; i++) {
		for (int j = 0; j < Img.cols; j++) {
			if (data[i * Img.cols + j][0] == 255 || data[i * Img.cols + j][1] == 255 || data[i * Img.cols + j][2] == 255)  // 빨간색 맞으면
			{
				data[i * Img.cols + j] = Vec3b(255, 255, 255); // 흰색으로 바꿔
			}
		}
	}
}
void copy(const Mat &Img, Mat &result, Mat &mask)
{
	Vec3b* maskData = (Vec3b*)mask.data;
	Vec3b* ImgData = (Vec3b*)Img.data;
	Vec3b* resData = (Vec3b*)result.data;
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (maskData[i * mask.cols + j] == Vec3b(255, 255, 255)) { // 흰색이라면! 붙이기
				resData[i * mask.cols + j] = ImgData[i * mask.cols + j];
			}
		}
	}
}
/*
void Mat::copyTo(OutputArray _dst, InputArray _mask) const
{
	Mat mask = _mask.getMat();
	if (!mask.data)
	{
		copyTo(_dst);
		return;
	}

	int cn = channels(), mcn = mask.channels();
	CV_Assert(mask.depth() == CV_8U && (mcn == 1 || mcn == cn));
	bool colorMask = mcn > 1;

	size_t esz = colorMask ? elemSize1() : elemSize();
	BinaryFunc copymask = getCopyMaskFunc(esz);

	uchar* data0 = _dst.getMat().data;
	_dst.create(dims, size, type());
	Mat dst = _dst.getMat();

	if (dst.data != data0) // do not leave dst uninitialized
		dst = Scalar(0);

	if (dims <= 2)
	{
		CV_Assert(size() == mask.size());
		Size sz = getContinuousSize(*this, dst, mask, mcn);
		copymask(data, step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
		return;
	}

	const Mat* arrays[] = { this, &dst, &mask, 0 };
	uchar* ptrs[3];
	NAryMatIterator it(arrays, ptrs);
	Size sz((int)(it.size*mcn), 1);

	for (size_t i = 0; i < it.nplanes; i++, ++it)
		copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}
*/
