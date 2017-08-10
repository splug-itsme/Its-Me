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
// 만약에 겹쳐있으면 그것도 하나의 객체로 취급하도록하는 그러면 위치값을 여러개를 가지도록 vector만기들