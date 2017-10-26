#ifndef _PERSONSET_H
#define _PERSONSET_H
#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include "analyze_Emtoion.h"
#include "yolo_v2_class.hpp"	// imported functions from DLL

#include <opencv2/opencv.hpp>			// C++
#pragma comment(lib, "opencv_world310.lib")

using namespace std;

typedef struct Obj_t {
	bbox_t vec;
	cv::Mat frame;
}obj_t;
class PersonSet { // Group people in the image and call emotion analyze = have people to put result background
	
	std::vector <obj_t> object; // person set
	std::vector <std::vector <obj_t> > tmpObject; // for calculating emotion
	std::vector <std::vector <obj_t> > groupPerson; // Adjacent people into groups
	std::vector <cv::Mat> groupFrame; // groupPerson Img
	std::vector <int> groupNum; // number of groups
public:
	PersonSet::PersonSet();
	PersonSet::PersonSet(cv::Mat &Img, std::vector <cv::Mat> &imgSet, std::vector<bbox_t> vec); // for detect emotion
	PersonSet::PersonSet(cv::Mat &Img, std::vector<bbox_t> vec); // not detect emotion
	PersonSet::~PersonSet();
	void make_tmpObject(cv::Mat &Img, bbox_t vec, int imgNum, int objNum);
	void add_Person(cv::Mat &Img, bbox_t vec);
	obj_t get_Person(int cnt);
	std::vector <obj_t> get_Group(int cnt); // Group adjacent people
	int size();
	void make_Group(std::vector <obj_t> object);
	bool cover_Area(bbox_t vec1, bbox_t vec2);
	void make_Groupframe(cv::Mat &firstFrame);
	void cal_Emotion();
	cv::Mat get_Frame(int cnt);
	void draw_Rect(cv::Mat &dst, cv::Mat &src, bbox_t vec);
	void remove_Objects(int fileNum, std::vector<char *>  fnameVec, char * folder);

	//void remove_Objects(int fileNum, std::vector<char *>  fnameVec);
};


#endif // PERSON_H