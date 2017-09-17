//#pragma once
#ifndef _FACE_CHANGE
#define _FACE_CHANGE

#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdio>
#include <math.h>

#include <opencv2/opencv.hpp>			// C++



#define M_PI       3.14159265358979323846   // pi
using namespace std;
using namespace dlib;


typedef matrix<double, 4556, 1> sample_type;
typedef radial_basis_kernel<sample_type> kernel_type;
typedef probabilistic_decision_function<kernel_type> probabilistic_funct_type;
typedef normalized_function<probabilistic_funct_type> pfunct_type;





class face_change {
public:
	int faceN;

	face_change::face_change();
	face_change::face_change(std::vector<char *> filename);
	face_change::face_change(std::vector<cv::Mat> face);
	int detectFaceAndCrop(char *imageName);
	//int detectFaceAndCrop(cv::Mat &des);
	std::vector<sample_type> getAllAttributes(int noOfFaces);
	std::vector<sample_type> getAllAttributes(cv::Mat &face);

	double length(point a, point b);
	double slope(point a, point b);
	void removePhotos();
	std::vector<double> svmMulticlass(sample_type sample);
	std::vector<double> probablityCalculator(std::vector<double> P);
	int find_Num(std::vector<double> happySize);

};

#endif