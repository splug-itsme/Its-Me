#include"backGround.h"

Detector detector("yolo.cfg", "yolo.weights");
float cutRate = 0.95; // 잘라낼 테두리 비율

//int main(int argc, char *argv[])
/* 배경을 구하기 위해서 영상을 읽어와 yolo 실행 및 배경 연산 수행
*/
backGround::backGround()
{
	
	//msgBox.show();
	//detector= detector("yolo.cfg", "yolo.weights");
	char videoFile[50] = "1.mp4";
	
	Mat des = sub_Bground(videoFile);
	capture_ROI(des, videoFile, "AA.mp4");
	add_ObjectToRes(des, videoFile);
	//	//	; // break에서 바꿈 키입력받을때마다 프레임이동

	//	/*gray("3.mp4");*/
	//	cvDestroyWindow("tt");

	//return 0;
}
// res Mat에 흰색으로 사람을 채운다. 즉 마스킹하기
void backGround::make_Mask(Mat &res, vector<bbox_t> const result_vec)
{
	Vec3b* resMat = (Vec3b*)res.data;
	for (auto &i : result_vec) {
		Point pt(i.x, i.y);
		Rect rect(pt, Size(i.w, i.h));
		rectangle(res, rect, Scalar(255, 255, 255), FILLED);
	}
	//	imshow("Mask", res);
}
void backGround::make_Mask(Mat &res, bbox_t const result_vec)
{
	Point pt(result_vec.x, result_vec.y);
	Rect rect(pt, Size(result_vec.w, result_vec.h));
	rectangle(res, rect, Scalar(255, 255, 255), FILLED);

	//imshow("Mask", res);
}
// 배경에 객체를 center좌표를 중심으로 붙여넣는다.
Mat backGround::add_object(Mat &background, Mat &object, Point center) {
	// center is object's center location
	Mat src_mask = 255 * Mat::ones(object.rows, object.cols, object.depth());
	Mat result;
	seamlessClone(object, background, src_mask, center, result, NORMAL_CLONE);
	return result;
}
// 배경구하기
Mat backGround::sub_Bground(char *videoFile)
{
	
	VideoCapture bgrCapture(videoFile);  // 영상 파일 읽기

	vector<Mat> src;
	Mat des;


	if (!bgrCapture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << "tt" << endl;
		exit(EXIT_FAILURE);
	}


	Mat Img; // 첫프레임 저장할 Mat
	bgrCapture.read(Img); // 첫프레임을 Img에 저장
	Mat readImg = Mat(Img.rows, Img.cols, CV_32SC3, Scalar(0, 0, 0)); //  읽은 이미지를 저장할 Mat
	des = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0)); // 결과를 저장할 Mat

	int check = 0;
	vector <Mat> frame;

	while (bgrCapture.read(readImg))
	{
		check++;
		if (check % 4 != 0)
			continue;
		vector<bbox_t> result_vec = detector.detect(readImg);
		Mat diff = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0)); // 객체를 흰색으로 채운후 저장할 Mat
		Mat result = Mat(Img.rows, Img.cols, CV_32SC3, Scalar(0, 0, 0)); // 객체를 제외한 사진을 가져와 저장

		//cvtColor(tmp, tmp, CV_BGR2GRAY);

		//absdiff(Img, readImg, diff); // 첫 프레임과 매 프레임마다의 차영상을 diff에 저장
		//threshold(diff, tmp, 20, 255, CV_THRESH_BINARY); // 차영상을 이진화 시켜서 tmp에 저장
		//changeGray(tmp); // 이진화 시킨 tmp를 완벽하게 이진화
		//pMOG2->apply(tmp, diff); // 이진화 시킨 것을 가지고 MOG2
		//erode(diff, diff, Mat(), Point(-1, -1), 3);
		//blur(diff, diff, Size(5, 5)); // noise 제거
		//diff = ~diff;
		//readImg.copyTo(result, diff); // 물체 영역의 반전을 붙여넣는다.

		make_Mask(diff, result_vec); // diff에 객체를 흰색으로 채워넣는다.
		copyMask(readImg, result, diff); // 물체 영역의 반전을 붙여넣는다.

		frame.push_back(result); // frame vector에 result를 모은다.
		//if (cvWaitKey() == 27) continue; // break에서 바꿈 키입력받을때마다 프레임이동

	}
	int size = frame[0].rows / 4; // thread 생성
	thread t1(&cal_Degree, des, frame, 0, size);
	thread t2(&cal_Degree, des, frame, size, size * 2);
	thread t3(&cal_Degree, des, frame, size * 2, size * 3);
	thread t4(&cal_Degree, des, frame, size * 3, size * 4);
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	check_Mat(des); // 배경에 300으로 채워진 수를 계산 즉 배경을 못 구한부분

	bgrCapture.release();
	//imshow("des", des);
	imwrite("bground.bmp", des);
	bg=des;
	return des;
}
// 배경에 사람을 그려넣는다. des는 배경을 저장, 
void backGround::add_ObjectToRes(Mat &des, char *filename) { 
	VideoCapture Capture("AA.mp4");  // 영상 파일 읽기
	Mat Img, AImg;
	if (!Capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << "tt" << endl;
		exit(EXIT_FAILURE);
	}
	VideoCapture bgrCapture(filename);  // 영상 파일 읽기
	if (!Capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << "tt" << endl;
		exit(EXIT_FAILURE);
	}
	Capture.read(AImg); // 테두리가 배경으로 채워진 영상에서 읽어서 저장
	bgrCapture.read(Img); // 원본 영상에서 첫프레임을 Img에 저장

	vector<bbox_t> first_vec = detector.detect(Img); // 첫프레임의 사람 위치 저장

	person=Person(AImg, first_vec); // 첫프레임의 사람 가져오기
	//imshow("des", des);
	//person.get_Person(0).frame;

	for (int k = 0; k < person.size(); k++) {
		int c;
		vector<obj_t> group = person.get_Group(k);

		// 그룹을 모아놓은 검은배경의 이미지
		Mat groupImg = Mat(AImg.rows, AImg.cols, CV_8UC3, Scalar(0, 0, 0));
		Mat tmp = Mat(AImg.rows, AImg.cols, CV_8UC3, Scalar(0, 0, 0)); // 사람이 있는 사각형을 흰색으로 마스킹 이미지가 저장된다.
		Rect A = Rect(group[0].vec.x, group[0].vec.y, group[0].vec.w - 1, group[0].vec.h - 1); // 첫번째 사람의 사각이미지를 가져온다.
		Rect B;
		for (int j = 0; j < group.size(); j++) {
			B = Rect(group[j].vec.x, group[j].vec.y, group[j].vec.w - 1, group[j].vec.h - 1);
			A = A | B; // 사각형을 or연산한다.
			rectangle(tmp, B, Scalar(255, 255, 255), FILLED); 
			AImg.copyTo(groupImg, tmp); // 사각형 범위의 이미지를 가져와서 groupImg에 저장
		}
		groupImg = groupImg(Rect(A));
		//imshow("group", groupImg);
		/*while (c = cvWaitKey()) {
			if (c == 27 || c == 'q')
				break;
			else {
				for (int i = 0; i < group.size(); i++) // 그룹의 모든 이미지 집어 넣기
					des = add_object(des, group[i].frame, Point(group[i].vec.x + group[i].vec.w / 2, group[i].vec.y + group[i].vec.h / 2));

				imshow("des", des);
				break;
			}
		}*/


	}
	//if (cvWaitKey() == 27);

	imwrite("result.bmp", des); // 자르기 전 결과이미지 저장
	des = des(Rect(des.cols * (1 - cutRate) / 2, des.rows * (1 - cutRate) / 2, des.cols * cutRate, des.rows * cutRate));
	imwrite("resultV2.bmp", des); // 자른 후 결과 이미지 저장

	Capture.release();
	bgrCapture.release();

}
// 일치도 정도를 연산한다. des는 결과를 저장, frame에는 계산을 위한 이미지 vector, start와 end는 일치도를  계산할 이미지의 구간 여기서는 0에서 최대를 단일구간으로 잡는다. 
void backGround::cal_Degree(Mat &des, vector <Mat> &frame, int start, int end) {  // 단일 구간의 프레임들 , des는 구간의 대표값
	printf("%d %d %d\n", frame.size(), start, end);
	vector <Mat> tmp;
	tmp.resize(frame.size());

	for (int i = 0; i < frame.size(); i++)
		frame[i].convertTo(tmp[i], CV_8UC3);

	Vec3b* desMat = (Vec3b*)des.data;
	for (int i = start; i < end; i++) {
		for (int j = 0; j < frame[0].cols; j++) {
			vector <int> agrDegree(frame.size(), 0);
			for (int k = 0; k < frame.size(); k++) {
				for (int t = k + 1; t < frame.size(); t++) {
					Vec3i* data = (Vec3i*)frame[k].data;
					Vec3i* data2 = (Vec3i*)frame[t].data;
					if (data[i * frame[0].cols + j] == Vec3i(300, 300, 300)) {
						continue;
					}
					else if (abs(data[i * frame[0].cols + j][0] - data2[i * frame[0].cols + j][0]) < 10 && abs(data[i * frame[0].cols + j][1] - data2[i * frame[0].cols + j][1]) < 10 && abs(data[i * frame[0].cols + j][2] - data2[i * frame[0].cols + j][2]) < 10) { // DEGREE보다 작으면 증가
						agrDegree[k]++;
						agrDegree[t]++;
					}
				}
			}
			int ret = return_Max(agrDegree);
			Vec3b* tmpData = (Vec3b*)tmp[ret].data;
			desMat[i * frame[0].cols + j] = tmpData[i * frame[0].cols + j];
		}
	}
	//imwrite("result.bmp", des);
}
// 300으로 채워진 값이 이미지에 얼마나 있는지 계산
void backGround::check_Mat(Mat &mat)
{
	Vec3b* matData = (Vec3b*)mat.data;
	int k = 0;
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			if (matData[i * mat.cols + j] == Vec3b(300, 300, 300)) {
				k++;
			}

		}
	}
	printf("k : %d\n", k);
}
// 일치도 최대값을 가진 프레임 번호 반환
int backGround::return_Max(vector <int> &agrDegree) 
{
	int ret = *max_element(agrDegree.begin(), agrDegree.end()); // 일치도 최대 구하기

	for (int i = 0; i < agrDegree.size(); i++) { // 이진검색하면 시간좀 줄일수 잇음
		if (agrDegree[i] == ret)
			return i;
	}
	return 0;
}

// 채널 3개의 이미지를 이진화 시킨다.
void backGround::changeGray(Mat &Img) 
{
	Vec3b* data = (Vec3b*)Img.data;
	for (int i = 0; i < Img.rows; i++) {
		for (int j = 0; j < Img.cols; j++) {
			if (data[i * Img.cols + j][0] == 255 || data[i * Img.cols + j][1] == 255 || data[i * Img.cols + j][2] == 255)  // 빨간색 맞으면
			{
				data[i * Img.cols + j] = Vec3b(255, 255, 255); // 흰색으로 바꿔
			}
		}
	}
}
//copyTo 변형 mask위치를 Img에서 가져와 result에 붙인다. 그리고 mask의 검은 부분은 300으로 채운다.
void backGround::copyMask(Mat &Img, Mat &result, Mat &mask) // copyTo 구현
{
	Img.convertTo(Img, CV_32SC3);

	//uchar* maskData = (uchar*)mask.data;
	Vec3b* maskData = (Vec3b*)mask.data;
	Vec3i* ImgData = (Vec3i*)Img.data;
	Vec3i* resData = (Vec3i*)result.data;

	//for (int i = 0; i < mask.rows; i++) {
	//	for (int j = 0; j < mask.cols; j++) {
	//		if (maskData[i * mask.cols + j] != uchar(128)) { // 흰색이라면! 붙이기
	//			resData[i * mask.cols + j] = ImgData[i * mask.cols + j];
	//		}
	//		else { // 검은색 부분은 300으로 채우기
	//			resData[i * mask.cols + j] = Vec3i(300, 300, 300);
	//		}
	//	}
	//}
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (maskData[i * mask.cols + j] != Vec3b(255, 255, 255)) { // 흰색이라면! 붙이기
				resData[i * mask.cols + j] = ImgData[i * mask.cols + j];
			}
			else { // 검은색 부분은 300으로 채우기
				resData[i * mask.cols + j] = Vec3i(300, 300, 300);
			}
		}
	}
}
/* 영상의 테두리를 잘라서 배경으로 채운 뒤 savefile에 저장
des는 배경이미지, videofile은 원본영상 savefile은 테두리를 자른뒤 저장될 파일 이름 */
void backGround::capture_ROI(Mat &des, char *videoFile, char *saveFile)
{
	VideoCapture capture(videoFile);  // 영상 파일 읽기
	VideoWriter vw;

	vw = VideoWriter(saveFile, CV_FOURCC('D', 'I', 'V', 'X'), capture.get(CV_CAP_PROP_FPS), Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH), (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT)), true);

	Mat Img = Mat(capture.get(CV_CAP_PROP_FRAME_HEIGHT), capture.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3, Scalar(0, 0, 0));
	Mat desMask = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));
	Mat resMask = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));
	Mat res = Mat(Img.rows, Img.cols, CV_8UC3, Scalar(0, 0, 0));


	Point pt(Img.cols * (1 - cutRate) / 2, Img.rows * (1 - cutRate) / 2);
	Rect rect(pt, Size(Img.cols * cutRate, Img.rows * cutRate));
	rectangle(desMask, rect, Scalar(255, 255, 255), FILLED);
	rectangle(resMask, rect, Scalar(255, 255, 255), FILLED);
	desMask = ~desMask;

	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << "tt" << endl;
		exit(EXIT_FAILURE);
	}
	if (!vw.isOpened())
	{
		cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;
		exit(1);
	}
	while (capture.read(Img)) {
		des.copyTo(res, desMask);
		Img.copyTo(res, resMask);
		vw.write(res);
	}
	vw.release();
}