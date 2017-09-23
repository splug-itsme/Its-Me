#ifndef _SUB_BACKGROOUND_H
#define _SUB_BACKGROOUND_H
#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <fstream>
#include <thread>

#define OPENCV
#include "yolo_v2_class.hpp"	// imported functions from DLL
#include "PersonSet.h"

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
#define RECURSIVE_COUNT 30

using namespace std;

class sub_Background {

public:

	sub_Background::sub_Background(char *videoFile);
	PersonSet person_set;
	cv::Mat bg, big_backImg;
	cv::Mat cal_Background(char *videoFile);
	void change_ColorToGray(cv::Mat &Img);
	void copy_MaskToImg(cv::Mat &Img, cv::Mat &result, cv::Mat &mask);
	static void sub_Background::cal_Degree(cv::Mat &des, std::vector <cv::Mat> &frame, int start, int end);
	static int sub_Background::return_Max(std::vector <int> &agrDegree);
	cv::Mat add_object(cv::Mat &sub_Background, cv::Mat &object, cv::Point center);
	void make_Mask(cv::Mat &res, std::vector<bbox_t> const result_vec);
	void make_Mask(cv::Mat &res, bbox_t const result_vec);
	void sub_Background::check_Mat(cv::Mat &mat);
	void capture_ROI(cv::Mat &des, char *videoFile, char *saveFile);
	void save_ROI(cv::Mat &Img, const std::vector <bbox_t> vec);
	void add_ObjectToRes(cv::Mat &des, char *filename);
};

#endif
