#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <fstream>
#include <thread>

#define OPENCV
#include "yolo_v2_class.hpp"	// imported functions from DLL
#include "Person.h"

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

class backGround {

public:
	
	backGround::backGround();

	/*Background subtract*/
	Mat sub_Bground(char *videoFile);
	void changeGray(Mat &Img);
	void copyMask(Mat &Img, Mat &result, Mat &mask);
	static void backGround::cal_Degree(Mat &des, vector <Mat> &frame, int start, int end);
	static int backGround::return_Max(vector <int> &agrDegree);
	Mat add_object(Mat &background, Mat &object, Point center);
	void make_Mask(Mat &res, vector<bbox_t> const result_vec);
	void make_Mask(Mat &res, bbox_t const result_vec);
	void backGround::check_Mat(Mat &mat);
	void capture_ROI(Mat &des, char *videoFile, char *saveFile);
	void add_ObjectToRes(Mat &des, char *filename);
};