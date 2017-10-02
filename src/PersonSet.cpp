#include "PersonSet.h"


using namespace cv;
PersonSet::PersonSet()
{

}
PersonSet::PersonSet(cv::Mat &Img, std::vector <cv::Mat> &imgSet, std::vector<bbox_t> vec) // make group after detect emotion
{
	tmpObject.resize(vec.size());
	for (int i = 0; i < imgSet.size(); i++) {
		for (int j = 0; j < vec.size(); j++) {
			make_tmpObject(imgSet[i], vec[j], i, j);
		}
	}
	cal_Emotion();
	groupNum.resize(object.size());
	groupPerson.resize(object.size());

	for (int i = 0; i < groupNum.size(); i++)
		groupNum[i] = i;

	make_Group(object);
	make_Groupframe(Img);
	//  insert the best object
}


PersonSet::PersonSet(cv::Mat &Img, std::vector<bbox_t> vec) // make group without detecting emotion
{
	for (auto &i : vec) {
		add_Person(Img, i);
	}
	groupNum.resize(object.size());
	groupPerson.resize(object.size());

	for (int i = 0; i < groupNum.size(); i++)
		groupNum[i] = i;

	make_Group(object);
	make_Groupframe(Img);

}

PersonSet::~PersonSet()
{
}
int PersonSet::size() { // return group size
	return groupPerson.size();
}
void PersonSet::make_tmpObject(cv::Mat &Img, bbox_t vec, int imgNum, int objNum)
{
	obj_t tmp;
	char filename[20];

	cv::Mat tmpImg;
	Img.copyTo(tmpImg);
	vec.x = max((int)vec.x - 20, 0);
	vec.y = max((int)vec.y - 50, 0);
	vec.h = vec.h + 100;
	vec.w = vec.w + 50;
	if (vec.x + vec.w > Img.cols)
		vec.w = Img.cols - vec.x;
	if (vec.y + vec.h > Img.rows)
		vec.h = Img.rows - vec.y;
	tmp.frame = tmpImg(Rect(vec.x, vec.y, vec.w - 1, vec.h - 1));
	tmp.vec = vec;
	tmpObject[objNum].push_back(tmp);
}
void PersonSet::cal_Emotion() { // detect emotion
	for (int objectNum = 0; objectNum < tmpObject.size(); objectNum++) { // Number of objects
		std::vector <char *> fnameVec(tmpObject[objectNum].size());
		std::vector <cv::Mat> fnameMat(tmpObject[objectNum].size());

		for (int i = 0; i < tmpObject[objectNum].size(); i++) { // repeat number of image
			fnameVec[i] = new char[100];
			sprintf(fnameVec[i], "file_%d_%d.jpg", i, objectNum);
			imwrite(fnameVec[i], tmpObject[objectNum][i].frame);
		}
		analyze_Emtoion ff = analyze_Emtoion(fnameVec); // return happiest image num

		if (ff.faceN == -1) // if cant detect face
			object.push_back(tmpObject[objectNum][0]);
		else
			object.push_back(tmpObject[objectNum][ff.faceN]);
		
		imwrite("emotion.bmp", object[objectNum].frame);
		imwrite("original.bmp", tmpObject[objectNum][0].frame);
#ifndef SAVE_TEST_FILES // Save intermediate output for test
		remove_Objects(objectNum, fnameVec);
#endif

	}
}
void PersonSet::remove_Objects(int fileNum, std::vector<char *>  fnameVec)
{
	int i;
	stringstream s;

	for (i = 0; i < fnameVec.size(); i++)
	{
		s.str("");
		s << "file_" << i <<"_" << fileNum <<  ".jpg";
		remove(s.str().c_str());
	}
}

void PersonSet::add_Person(cv::Mat &Img, bbox_t vec) // make object (= push person)
{
	obj_t tmp;
	cv::Mat tmpImg;
	Img.copyTo(tmpImg);
	vec.x = max((int)vec.x - 20, 0);
	vec.y = max((int)vec.y - 50, 0);
	vec.h = vec.h + 80;
	vec.w = vec.w + 20;
	if (vec.x + vec.w > Img.cols)
		vec.w = Img.cols - vec.x;
	if (vec.y + vec.h > Img.rows)
		vec.h = Img.rows - vec.y;
	tmp.frame = tmpImg(Rect(vec.x, vec.y, vec.w - 1, vec.h - 1));
	tmp.vec = vec;
	object.push_back(tmp);
}

obj_t PersonSet::get_Person(int cnt) // get n.th person
{
	return object.at(cnt);
}
std::vector <obj_t> PersonSet::get_Group(int cnt)
{
	return groupPerson.at(cnt);
}
Mat PersonSet::get_Frame(int cnt) {
	return groupFrame.at(cnt);
}

void PersonSet::make_Group(std::vector <obj_t> object) // make Group with object set
{
	for (int i = 0; i < object.size(); i++) {
		for (int j = i + 1; j < object.size(); j++) {
			if (cover_Area(object[i].vec, object[j].vec) == true)
			{
				groupNum[j] = groupNum[i];
			}
		}
	}
	for (int i = 0; i < object.size(); i++) {
		groupPerson[groupNum[i]].push_back(object[i]);
	}
	for (int i = 0; i < groupPerson.size();) {
		if (groupPerson[i].size() == 0) {
			groupPerson.erase(groupPerson.begin() + i);
		}
		else
			i++;
	}
}
bool PersonSet::cover_Area(bbox_t vec1, bbox_t vec2) { // Confirm adjacent 
	Rect A(vec1.x, vec1.y, vec1.w - 1, vec1.h - 1);
	Rect B(vec2.x, vec2.y, vec2.w - 1, vec2.h - 1);
	Rect C = A & B;
	if (C.width == 0 || C.height == 0)
		return false;
	return true;
}
void PersonSet::make_Groupframe(cv::Mat &firstFrame) // make groupImg to show
{
	for (int k = 0; k < groupPerson.size(); k++) {
		int c;
		std::vector<obj_t> group = groupPerson.at(k);

		// 그룹을 모아놓은 검은배경의 이미지
		Mat groupImg = Mat(firstFrame.rows, firstFrame.cols, CV_8UC3, Scalar(0, 0, 0));
		Rect A = Rect(group[0].vec.x, group[0].vec.y, group[0].vec.w - 1, group[0].vec.h - 1);
		Rect B;
		for (int j = 0; j < group.size(); j++) {
			B = Rect(group[j].vec.x, group[j].vec.y, group[j].vec.w - 1, group[j].vec.h - 1);
			A = A | B;
			draw_Rect(groupImg, group[j].frame, group[j].vec);
		}
		groupImg = groupImg(Rect(A));
		groupFrame.push_back(groupImg);
	}

}
void PersonSet::draw_Rect(cv::Mat &dst, cv::Mat &src, bbox_t vec)
{
	cv::Vec3b* dstData = (cv::Vec3b*)dst.data;
	cv::Vec3b* srcData = (cv::Vec3b*)src.data;
	for (int i = 0; i < vec.h; i++) {
		for (int j = 0; j < vec.w; j++) {
			dstData[vec.x + vec.y * dst.cols + i * dst.cols + j] = srcData[i * dst.cols + j];
		}
	}
}
