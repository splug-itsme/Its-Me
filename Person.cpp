#include "Person.h"



Person::Person()
{

}
Person::Person(Mat &Img, vector<bbox_t> vec)
{
	for (auto &i : vec) {
		add_Person(Img, i);
	}
}

Person::~Person()
{
}
int Person::size() {
	return object.size();
}
void Person::add_Person(Mat &Img, bbox_t vec)
{
	obj_t tmp;
	Mat tmpImg;
	Img.copyTo(tmpImg);
	vec.y = max((int)vec.y - 50, 0);
	vec.h = vec.h + 80;
	if (vec.x + vec.w > Img.cols)
		vec.w = Img.cols - vec.x;
	if (vec.y + vec.h > Img.rows)
		vec.h = Img.rows - vec.y;
	tmp.frame = tmpImg(Rect(vec.x, vec.y, vec.w-1, vec.h-1));
	tmp.vec = vec;
	object.push_back(tmp);

}

obj_t Person::get_Person(int cnt)
{
	return object.at(cnt);
}

// 만약에 겹쳐있으면 그것도 하나의 객체로 취급하도록하는 그러면 위치값을 여러개를 가지도록 vector만기들