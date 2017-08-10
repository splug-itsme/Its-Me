#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <fstream>
#include <thread>

#include "yolo_v2_class.hpp"	// imported functions from DLL

#include <opencv2/opencv.hpp>			// C++
#pragma comment(lib, "opencv_world310.lib")

using namespace std;
using namespace cv;

typedef struct Obj_t {
	bbox_t vec;
	Mat frame;
}obj_t;
class Person {
	vector <obj_t> object;
	vector <vector < obj_t> > groupPerson;
	vector <int> groupNum;
public:
	Person::Person();
	Person::Person(Mat &Img, vector<bbox_t> vec);
	Person::~Person();
	void add_Person(Mat &Img, bbox_t vec);
	obj_t get_Person(int cnt);
	vector <obj_t> get_Group(int cnt);
	int size();
	void make_Group();
	bool cover_Area(bbox_t vec1, bbox_t vec2);
};