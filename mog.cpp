#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <fstream>
#include <thread>

#define OPENCV

#include "yolo_v2_class.hpp"	// imported functions from DLL


#ifdef OPENCV
#include <opencv2/opencv.hpp>			// C++
#pragma comment(lib, "opencv_world310.lib")
#endif

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
#include <sstream>
#include <algorithm>

#define DEGREE 10

using namespace cv;
using namespace std;

int sub_Bground(char *videoFile);
void changeGray(Mat &Img);
void cvDiff(Mat &image, Mat &image2, Mat &diff);
void copyMask(Mat &Img, Mat &result, Mat &mask);
void cal_Degree(vector <Mat> &frame, int start, int end);
int return_Max(vector <int> &agrDegree);


void make_Mask(Mat &res, vector<bbox_t> const result_vec);
void check_Mat(Mat &mat);
Mat des;




void show_result(std::vector<bbox_t> const result_vec, std::vector<std::string> const obj_names) {
	for (auto &i : result_vec) {
		if (obj_names.size() > i.obj_id) std::cout << obj_names[i.obj_id] << " - ";
		std::cout << "obj_id = " << i.obj_id << ",  x = " << i.x << ", y = " << i.y
			<< ", w = " << i.w << ", h = " << i.h
			<< std::setprecision(3) << ", prob = " << i.prob << std::endl;
	}
}

std::vector<std::string> objects_names_from_file(std::string const filename) {
	std::ifstream file(filename);
	std::vector<std::string> file_lines;
	if (!file.is_open()) return file_lines;
	for (std::string line; file >> line;) file_lines.push_back(line);
	std::cout << "object names loaded \n";
	return file_lines;
}


int main(int argc, char *argv[])
{

	auto obj_names = objects_names_from_file("data/voc.names");
	//cvNamedWindow("Image");
	//cvNamedWindow("Image2");
	//cvNamedWindow("tmpImage");
	//cvNamedWindow("diffImage");
	//cvNamedWindow("resultImage");
	if (sub_Bground("2.mp4") == 1)
		return 1;
	if (cvWaitKey() == 27)
		//	; // break에서 바꿈 키입력받을때마다 프레임이동

		/*gray("3.mp4");*/
		cvDestroyWindow("tt");

	return 0;
}
void make_Mask(Mat &res, vector<bbox_t> const result_vec)
{
	Vec3b* resMat = (Vec3b*)res.data;
	for (auto &i : result_vec) {
		/*for (int x = i.x; x < i.x + i.w, x < res.cols; x++) {
		for (int y = i.y; y < i.y + i.h, y < res.rows ; y++) {
		resMat[y * res.cols + x] = Vec3b(255, 255, 255);
		}
		}*/
		Point pt(i.x, i.y);
		Rect rect(pt, Size(i.w, i.h));
		rectangle(res, rect, Scalar(255, 255, 255), FILLED);
	}
	imshow("Mask", res);
}

int sub_Bground(char *videoFile)
{
	Detector detector("yolo.cfg", "yolo.weights");
	VideoCapture capture(videoFile);  // 영상 파일 읽기

	vector<Mat> src;

	/*if (!vw.isOpened())
	{
	cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;
	return 1;
	}*/
	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << "tt" << endl;
		exit(EXIT_FAILURE);
	}


	Mat Img;
	capture.read(Img); // 처프레임을 Img에 저장

					   //Mat Img = imread("result.bmp"); // Img에 결과로 얻은 이미지 저장
	Mat readImg = Mat(Img.rows, Img.cols, CV_32SC3, Scalar(0, 0, 0)); // 
																	  //Mat diff = Mat(Img.rows, Img.cols, CV_32SC3, Scalar(0, 0, 0));

	Mat tmp = Mat(Img.rows, Img.cols, CV_32SC3, Scalar(0, 0, 0));
	des = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0)); // 결과를 저장할 Mat

	Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
	pMOG2 = createBackgroundSubtractorMOG2();
	int check = 0;
	vector <Mat> frame;

	while (capture.read(readImg))
	{
		check++;
		if (check % 4 != 0)
			continue;
		vector<bbox_t> result_vec = detector.detect(readImg);
		Mat diff = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));
		Mat result = Mat(Img.rows, Img.cols, CV_32SC3, Scalar(0, 0, 0));

		//cvtColor(tmp, tmp, CV_BGR2GRAY);

		//absdiff(Img, readImg, diff); // 첫 프레임과 매 프레임마다의 차영상을 diff에 저장
		//threshold(diff, tmp, 20, 255, CV_THRESH_BINARY); // 차영상을 이진화 시켜서 tmp에 저장
		//changeGray(tmp); // 이진화 시킨 tmp를 완벽하게 이진화

		//pMOG2->apply(tmp, diff); // 이진화 시킨 것을 가지고 MOG2
		//erode(diff, diff, Mat(), Point(-1, -1), 3);

		//blur(diff, diff, Size(5, 5)); // noise 제거
		//diff = ~diff;
		//readImg.copyTo(result, diff); // 물체 영역의 반전을 붙여넣는다.

		make_Mask(diff, result_vec);
		copyMask(readImg, result, diff); // 물체 영역의 반전을 붙여넣는다.

		frame.push_back(result);


		imshow("Image", Img);
		imshow("Image2", readImg);
		imshow("diffImage", diff);
		imshow("resultImage", result);

		//if (cvWaitKey() == 27) continue; // break에서 바꿈 키입력받을때마다 프레임이동

	}
	printf("frame : %d\n", frame.size());
	int size = frame[0].rows / 4; // thread 생성
	thread t1(&cal_Degree, frame, 0, size);
	thread t2(&cal_Degree, frame, size, size * 2);
	thread t3(&cal_Degree, frame, size * 2, size * 3);
	thread t4(&cal_Degree, frame, size * 3, size * 4);
	/*thread t5(&cal_Degree, frame, size * 4, size * 5);
	thread t6(&cal_Degree, frame, size * 5, size * 6);
	*/
	t1.join();
	t2.join();
	t3.join();
	t4.join();/*
			  t5.join();
			  t6.join();*/
	check_Mat(des);
	imwrite("result.bmp", des);
	capture.release();
	//vw.release();

	return 0;
}
void cal_Degree(vector <Mat> &frame, int start, int end) {  // 단일 구간의 프레임들 , des는 구간의 대표값
	printf("%d %d %d\n", frame.size(), start, end);
	vector <Mat> tmp;
	tmp.resize(frame.size());

	for (int i = 0; i < frame.size(); i++)
	{
		frame[i].convertTo(tmp[i], CV_8UC3);
	}
	Vec3b* desMat = (Vec3b*)des.data;
	for (int i = start; i < end; i++) {
		for (int j = 0; j < frame[0].cols; j++) {
			vector <int> agrDegree(frame.size(), 0);
			for (int k = 0; k < frame.size(); k++) {
				for (int t = k + 1; t < frame.size(); t++) {
					Vec3i* data = (Vec3i*)frame[k].data;
					Vec3i* data2 = (Vec3i*)frame[t].data;
					if (data[i * frame[0].cols + j] == Vec3i(300, 300, 300)) {
						continue;
					}
					else if (abs(data[i * frame[0].cols + j][0] - data2[i * frame[0].cols + j][0]) < 10 && abs(data[i * frame[0].cols + j][1] - data2[i * frame[0].cols + j][1]) < 10 && abs(data[i * frame[0].cols + j][2] - data2[i * frame[0].cols + j][2]) < 10) { // DEGREE보다 작으면 증가
						agrDegree[k]++;
						agrDegree[t]++;
					}
				}
			}
			int ret = return_Max(agrDegree);
			Vec3b* tmpData = (Vec3b*)tmp[ret].data;
			desMat[i * frame[0].cols + j] = tmpData[i * frame[0].cols + j];
		}
	}

	imwrite("Img.bmp", frame[0]);
	imwrite("Img2.bmp", frame[1]);
	imwrite("result.bmp", des);

}
void check_Mat(Mat &mat)
{
	Vec3b* matData = (Vec3b*)mat.data;
	int k = 0;
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			if (matData[i * mat.cols + j] == Vec3b(300, 300, 300)) {
				k++;
			}

		}
	}
	printf("k : %d\n", k);
}
int return_Max(vector <int> &agrDegree) // 일치도 최대값을 가진 프레임 번호 반환
{
	int ret = *max_element(agrDegree.begin(), agrDegree.end()); // 일치도 최대 구하기

	for (int i = 0; i < agrDegree.size(); i++) { // 이진검색하면 시간좀 줄일수 잇음
		if (agrDegree[i] == ret)
			return i;
	}
	return 0;
}

void cvDiff(Mat &image, Mat &image2, Mat &diff) { // 차영상
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


void changeGray(Mat &Img) // 채널 3개 이진화 하기
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
void copyMask(Mat &Img, Mat &result, Mat &mask) // copyTo 구현
{
	Img.convertTo(Img, CV_32SC3);

	//uchar* maskData = (uchar*)mask.data;
	Vec3b* maskData = (Vec3b*)mask.data;
	Vec3i* ImgData = (Vec3i*)Img.data;
	Vec3i* resData = (Vec3i*)result.data;

	//for (int i = 0; i < mask.rows; i++) {
	//	for (int j = 0; j < mask.cols; j++) {
	//		if (maskData[i * mask.cols + j] != uchar(128)) { // 흰색이라면! 붙이기
	//			resData[i * mask.cols + j] = ImgData[i * mask.cols + j];
	//		}
	//		else { // 검은색 부분은 300으로 채우기
	//			resData[i * mask.cols + j] = Vec3i(300, 300, 300);
	//		}
	//	}
	//}
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (maskData[i * mask.cols + j] != Vec3b(255, 255, 255)) { // 흰색이라면! 붙이기
				resData[i * mask.cols + j] = ImgData[i * mask.cols + j];
			}
			else { // 검은색 부분은 300으로 채우기
				resData[i * mask.cols + j] = Vec3i(300, 300, 300);
			}
		}
	}
}