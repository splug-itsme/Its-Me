#include "Person.h"



Person::Person()
{

}
Person::Person(Mat &Img, vector<bbox_t> vec) 
{
	for (auto &i : vec) {
		add_Person(Img, i);
	}
	groupNum.resize(object.size());
	groupPerson.resize(object.size());

	for (int i = 0; i < groupNum.size(); i++)
		groupNum[i] = i;

	make_Group();
	make_Groupframe(Img);

}

Person::~Person()
{
}
int Person::size() {
	return groupPerson.size();
}

void Person::add_Person(Mat &Img, bbox_t vec)
{
	obj_t tmp;
	Mat tmpImg;
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
vector <obj_t> Person::get_Group(int cnt)
{
	return groupPerson.at(cnt);
}
void Person::make_Group()
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
void Person::make_Groupframe(Mat &firstFrame)
{
	for (int k = 0; k < groupPerson.size(); k++) {
		int c;
		vector<obj_t> group = groupPerson.at(k);

		// 그룹을 모아놓은 검은배경의 이미지
		Mat groupImg = Mat(firstFrame.rows, firstFrame.cols, CV_8UC3, Scalar(0, 0, 0));
		Mat tmp = Mat(firstFrame.rows, firstFrame.cols, CV_8UC3, Scalar(0, 0, 0));;
		Rect A = Rect(group[0].vec.x, group[0].vec.y, group[0].vec.w - 1, group[0].vec.h - 1);
		Rect B;
		for (int j = 0; j < group.size(); j++) {
			B = Rect(group[j].vec.x, group[j].vec.y, group[j].vec.w - 1, group[j].vec.h - 1);
			A = A | B;
			rectangle(tmp, B, Scalar(255, 255, 255), FILLED);
			firstFrame.copyTo(groupImg, tmp);
		}
		groupImg = groupImg(Rect(A));
		groupFrame.push_back(groupImg);
		imshow("group", groupImg);
	}
}
// 만약에 겹쳐있으면 그것도 하나의 객체로 취급하도록하는 그러면 위치값을 여러개를 가지도록 vector만기들