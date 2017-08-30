#include "Person.h"


using namespace cv;
Person::Person()
{

}
Person::Person(cv::Mat &Img, std::vector <cv::Mat> &imgSet, std::vector<bbox_t> vec)
{
	tmpObject.resize(vec.size());
	for (int i = 0; i < imgSet.size(); i++) {
		for (int j = 0; j < vec.size(); j++) {
			make_tmpObject(imgSet[i], vec[j], i, j);
		}
	}
	cal_Emotion();
	groupNum.resize(optObject.size());
	groupPerson.resize(optObject.size());

	for (int i = 0; i < groupNum.size(); i++)
		groupNum[i] = i;

	make_Group(optObject);
	make_Groupframe(Img);
	// 여기에 최적의 object를 찾아서 넣도록 구현
}


Person::Person(cv::Mat &Img, std::vector<bbox_t> vec)
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

Person::~Person()
{
}
int Person::size() {
	return groupPerson.size();
}
void Person::make_tmpObject(cv::Mat &Img, bbox_t vec, int imgNum, int objNum)
{
	obj_t tmp;
	char filename[20];

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
	sprintf(filename, "file_%d_%d.jpg", imgNum, objNum);
	imwrite(filename, tmp.frame);
	tmpObject[objNum].push_back(tmp);
}
void Person::cal_Emotion() {
	for (int j = 0; j < tmpObject.size(); j++) { // 객체의 수
		std::vector <char *> fnameVec(tmpObject[j].size());
		for (int i = 0; i < tmpObject[j].size(); i++) { // 이미지의 수
			fnameVec[i] = new char[100];
			sprintf(fnameVec[i], "file_%d_%d.jpg", i, j);
		}
		face_change ff = face_change(fnameVec);
		printf("ff %d\n", ff.faceN);
		if (ff.faceN == -1)
			optObject.push_back(tmpObject[j][0]);
		else
			optObject.push_back(tmpObject[j][ff.faceN]);
		imwrite("emotion.bmp", optObject[j].frame);
		imwrite("original.bmp", tmpObject[j][0].frame);

	}
}

void Person::add_Person(cv::Mat &Img, bbox_t vec)
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

obj_t Person::get_Person(int cnt)
{
	return object.at(cnt);
}
std::vector <obj_t> Person::get_Group(int cnt)
{
	return groupPerson.at(cnt);
}
Mat Person::get_Frame(int cnt) {
	return groupFrame.at(cnt);
}

void Person::make_Group(std::vector <obj_t> object)
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
bool Person::cover_Area(bbox_t vec1, bbox_t vec2) {
	Rect A(vec1.x, vec1.y, vec1.w - 1, vec1.h - 1);
	Rect B(vec2.x, vec2.y, vec2.w - 1, vec2.h - 1);
	Rect C = A & B;
	if (C.width == 0 || C.height == 0)
		return false;
	return true;
}
void Person::make_Groupframe(cv::Mat &firstFrame)
{
	for (int k = 0; k < groupPerson.size(); k++) {
		int c;
		std::vector<obj_t> group = groupPerson.at(k);

		// 그룹을 모아놓은 검은배경의 이미지
		Mat groupImg = Mat(firstFrame.rows, firstFrame.cols, CV_8UC3, Scalar(0, 0, 0));
		Mat tmp = Mat(firstFrame.rows, firstFrame.cols, CV_8UC3, Scalar(0, 0, 0));
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
void Person::draw_Rect(cv::Mat &dst, cv::Mat &src, bbox_t vec) 
{
	cv::Vec3b* dstData = (cv::Vec3b*)dst.data;
	cv::Vec3b* srcData = (cv::Vec3b*)src.data;
	for (int i = 0; i < vec.h; i++) {
		for (int j = 0; j < vec.w; j++) {
			dstData[vec.x + vec.y * dst.cols + i * dst.cols + j] = srcData[i * dst.cols + j]; 
		}
	}
}

// 만약에 겹쳐있으면 그것도 하나의 객체로 취급하도록하는 그러면 위치값을 여러개를 가지도록 vector만기들