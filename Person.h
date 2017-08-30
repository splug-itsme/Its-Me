#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include "face_change.h"
#include "yolo_v2_class.hpp"	// imported functions from DLL

#include <opencv2/opencv.hpp>			// C++
#pragma comment(lib, "opencv_world310.lib")

using namespace std;

typedef struct Obj_t {
	bbox_t vec;
	cv::Mat frame;
}obj_t;
class Person {
	std::vector <obj_t> object;
	std::vector <obj_t> optObject;
	std::vector <std::vector <obj_t> > tmpObject;
	std::vector <std::vector < obj_t> > groupPerson;
	std::vector <cv::Mat> groupFrame;
	std::vector <int> groupNum;
public :
	Person::Person();
	Person::Person(cv::Mat &Img, std::vector <cv::Mat> &imgSet, std::vector<bbox_t> vec);
	Person::Person(cv::Mat &Img, std::vector<bbox_t> vec);
	Person::~Person();
	void make_tmpObject(cv::Mat &Img, bbox_t vec, int imgNum, int objNum);
	void add_Person(cv::Mat &Img, bbox_t vec);
	obj_t get_Person(int cnt);
	std::vector <obj_t> get_Group(int cnt);
	int size();
	void make_Group(std::vector <obj_t> object);
	bool cover_Area(bbox_t vec1, bbox_t vec2);
	void make_Groupframe(cv::Mat &firstFrame);
	void cal_Emotion();
	cv::Mat get_Frame(int cnt);
	void draw_Rect(cv::Mat &dst, cv::Mat &src, bbox_t vec);
};