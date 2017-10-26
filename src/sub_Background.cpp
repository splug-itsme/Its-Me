#include "sub_Background.h"

Detector detector("./data/yolo.cfg", "./data/yolo.weights");
float cutRate = 0.95; // Percentage to cut to Alpha Blending


/* Execute yolo and perform operations using image to make background  */
sub_Background::sub_Background(char *videoFile)
{
	cv::Mat des = cal_Background(videoFile);
	capture_ROI(des, videoFile, "AA.mp4");
	add_ObjectToRes(des, videoFile);
}
// White fills a person's area. That is, masking
void sub_Background::make_Mask(cv::Mat &res, std::vector<bbox_t> const result_vec)
{
	cv::Vec3b* resMat = (cv::Vec3b*)res.data;
	for (auto &i : result_vec) {
		cv::Point pt(i.x, i.y);
		cv::Rect rect(pt, cv::Size(i.w, i.h));
		cv::rectangle(res, rect, cv::Scalar(255, 255, 255), cv::FILLED);
	}
	//	imshow("Mask", res);
}
void sub_Background::make_Mask(cv::Mat &res, bbox_t const result_vec)
{
	cv::Point pt(result_vec.x, result_vec.y);
	cv::Rect rect(pt, cv::Size(result_vec.w, result_vec.h));
	cv::rectangle(res, rect, cv::Scalar(255, 255, 255), cv::FILLED);
}
// save ROI area
void sub_Background::save_ROI(cv::Mat &Img, const std::vector <bbox_t> vec)
{
	for (auto &i : vec) {
		cv::Point pt(i.x, i.y);
		cv::rectangle(Img, pt, cv::Point(i.x + i.w, i.y + i.h), cv::Scalar(0, 0, 255), 3);
	}
}
// Pastes objects in the background centered on the center coordinates.
cv::Mat sub_Background::add_object(cv::Mat &sub_Background, cv::Mat &object, cv::Point center) {
	// center is object's center location
	cv::Mat src_mask = 255 * cv::Mat::ones(object.rows, object.cols, object.depth());
	cv::Mat result;
	seamlessClone(object, sub_Background, src_mask, center, result, cv::NORMAL_CLONE);
	return result;
}
// Getting the background.
cv::Mat sub_Background::cal_Background(char *videoFile)
{
	cv::VideoCapture bgrCapture(videoFile);  // read video file
#ifdef SAVE_ROI_VIDEO // if you want save ROI video 
	cv::VideoWriter vw;
	vw = cv::VideoWriter("ROIvideo.mp4", CV_FOURCC('D', 'I', 'V', 'X'), 5, cv::Size((int)bgrCapture.get(CV_CAP_PROP_FRAME_WIDTH), (int)bgrCapture.get(CV_CAP_PROP_FRAME_HEIGHT)), true);
#endif

	std::vector<cv::Mat> src;
	cv::Mat des;

	if (!bgrCapture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << "tt" << endl;
		exit(EXIT_FAILURE);
	}


	cv::Mat Img;
	bgrCapture.read(Img); // Save the first frame to Img
	cv::Mat readImg = cv::Mat(Img.rows, Img.cols, CV_32SC3, cv::Scalar(0, 0, 0)); //  Mat to save the read image
	des = cv::Mat(Img.rows, Img.cols, CV_8UC3, cv::Scalar(0, 0, 0)); // Mat to save results

	int check = 0;
	std::vector <cv::Mat> frame;

	while (bgrCapture.read(readImg))
	{
		check++;
		if (check % 4 != 0)
			continue;
		std::vector<bbox_t> result_vec = detector.detect(readImg);
		cv::Mat diff = cv::Mat(Img.rows, Img.cols, CV_8UC3, cv::Scalar(0, 0, 0)); // Mat to fill the object with white,
		cv::Mat result = cv::Mat(Img.rows, Img.cols, CV_32SC3, cv::Scalar(0, 0, 0)); // Mat to save photos except objects
#ifdef SAVE_ROI_VIDEO
		cv::Mat tmp = readImg.clone();
#endif
		for (auto vec : result_vec) {
			vec.x = max((int)vec.x - 20, 0);
			vec.y = max((int)vec.y - 50, 0);
			vec.h = vec.h + 100;
			vec.w = vec.w + 50;
		}
#ifdef SAVE_ROI_VIDEO
		save_ROI(tmp, result_vec);
		vw.write(tmp);
#endif

		make_Mask(diff, result_vec); // Fill objects with white in diff.
		copy_MaskToImg(readImg, result, diff); // Pastes the inverse of the object area.
		
		frame.push_back(result);

	}
#ifdef SAVE_ROI_VIDEO
	vw.release();
#endif

	int size = frame[0].rows / 4; // create Thread, Compute to variance to reduce computation time
	thread t1(&cal_Degree, des, frame, 0, size);
	thread t2(&cal_Degree, des, frame, size, size * 2);
	thread t3(&cal_Degree, des, frame, size * 2, size * 3);
	thread t4(&cal_Degree, des, frame, size * 3, size * 4);
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	check_Mat(des); // Calculate the number filled in the background to 300. That is can;t get background area

	bgrCapture.release();
	imwrite("bground.bmp", des);
	bg = des;
	big_backImg = des(cv::Rect(des.cols * (1 - cutRate) / 2, des.rows * (1 - cutRate) / 2, des.cols * cutRate, des.rows * cutRate));
	return des;
}
// Draw a person in the background. des = background Img
void sub_Background::add_ObjectToRes(cv::Mat &des, char *filename) {
	cv::VideoCapture Capture("AA.mp4"); 
	cv::Mat first_Img, back_Img;

	if (!Capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << filename << endl;
		exit(EXIT_FAILURE);
	}
	cv::VideoCapture bgrCapture(filename);  
	if (!bgrCapture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << filename << endl;
		exit(EXIT_FAILURE);
	}
	Capture.read(back_Img);
	bgrCapture.read(first_Img); // Save the first frame from the original image to Img

	std::vector<bbox_t> first_vec = detector.detect(first_Img); // Save the first frame's person location
#ifdef DETECT_EMOTION
	std::vector <cv::Mat> emotion_ImgSet; // Image vector for emotion computation

	emotion_ImgSet.push_back(first_Img);
	int tmpNum = 0;
	for (int i = 0; i < RECURSIVE_COUNT; i++) { //  RECURSIVE_COUNT  = 30
		cv::Mat tmpImg;
		
		bgrCapture.read(tmpImg);
		tmpNum++;
		if (tmpNum % 2 == 1)
			continue;
		emotion_ImgSet.push_back(tmpImg);
	}
	person_set = PersonSet(back_Img, emotion_ImgSet, first_vec); //  emotion detect person
#else
	person_set = PersonSet(back_Img, first_vec); // NOT detect_emotion 
#endif

	des = des(cv::Rect(des.cols * (1 - cutRate) / 2, des.rows * (1 - cutRate) / 2, des.cols * cutRate, des.rows * cutRate));
	imwrite("sub_Background.bmp", des);

	Capture.release();
	bgrCapture.release();

}
// calculates the degree of matching save result Img to des, frame is Imgvector to calculate, start and end are interval of the image to be calculated.
void sub_Background::cal_Degree(cv::Mat &des, std::vector <cv::Mat> &frame, int start, int end)
{  // Single - section frames, 


	printf("%d %d %d\n", frame.size(), start, end);
	std::vector <cv::Mat> tmp; // Temporary Mat vector for computation
	tmp.resize(frame.size());

	for (int i = 0; i < frame.size(); i++)
		frame[i].convertTo(tmp[i], CV_8UC3);

	cv::Vec3b* desMat = (cv::Vec3b*)des.data; // Point to 3 - channel byte size Vec class type Img' data member 
	for (int i = start; i < end; i++) {
		for (int j = 0; j < frame[0].cols; j++) {
			std::vector <int> agrDegree(frame.size(), 0);
			for (int k = 0; k < frame.size(); k++) {
				for (int t = k + 1; t < frame.size(); t++) {
					cv::Vec3i* data = (cv::Vec3i*)frame[k].data; // Generate Vec as an integer type To compare values greater than 255(byte)
					cv::Vec3i* data2 = (cv::Vec3i*)frame[t].data;
					if (data[i * frame[0].cols + j] == cv::Vec3i(300, 300, 300)) {
						continue;
					}
					else if (abs(data[i * frame[0].cols + j][0] - data2[i * frame[0].cols + j][0]) < 10 && abs(data[i * frame[0].cols + j][1] - data2[i * frame[0].cols + j][1]) < 10 && abs(data[i * frame[0].cols + j][2] - data2[i * frame[0].cols + j][2]) < 10) { //  smaller than degree then increase.
						agrDegree[k]++;
						agrDegree[t]++;
					}
				}
			}
			int ret = return_Max(agrDegree);
			cv::Vec3b* tmpData = (cv::Vec3b*)tmp[ret].data;
			desMat[i * frame[0].cols + j] = tmpData[i * frame[0].cols + j];
		}
	}
}
// Calculate how many values are filled in the image with 300
void sub_Background::check_Mat(cv::Mat &mat)
{
	cv::Vec3b* matData = (cv::Vec3b*)mat.data;
	int k = 0;
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			if (matData[i * mat.cols + j] == cv::Vec3b(300, 300, 300)) {
				k++;
			}

		}
	}
	printf("300 data counts : %d\n", k);
}
// Returns the frame number with the maximum match value
int sub_Background::return_Max(std::vector <int> &agrDegree)
{
	int ret = *max_element(agrDegree.begin(), agrDegree.end()); 

	for (int i = 0; i < agrDegree.size(); i++) {
		if (agrDegree[i] == ret)
			return i;
	}
	return 0;
}

// Binarize the  3 - channel Img.
void sub_Background::change_ColorToGray(cv::Mat &Img)
{
	cv::Vec3b* data = (cv::Vec3b*)Img.data;
	for (int i = 0; i < Img.rows; i++) {
		for (int j = 0; j < Img.cols; j++) {
			if (data[i * Img.cols + j][0] == 255 || data[i * Img.cols + j][1] == 255 || data[i * Img.cols + j][2] == 255) 
			{
				data[i * Img.cols + j] = cv::Vec3b(255, 255, 255); // change to white
			}
		}
	}
}
//Take the mask position from Img and attach it to result. and the black part of the mask is filled with 300(Values ​​greater than 255)
void sub_Background::copy_MaskToImg(cv::Mat &Img, cv::Mat &result, cv::Mat &mask) // copyTo 구현
{
	Img.convertTo(Img, CV_32SC3);
	cv::Vec3b* maskData = (cv::Vec3b*)mask.data; 
	cv::Vec3i* ImgData = (cv::Vec3i*)Img.data; // generate Vec as an integer type To compare values greater than 255(byte)
	cv::Vec3i* resData = (cv::Vec3i*)result.data;

	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (maskData[i * mask.cols + j] != cv::Vec3b(255, 255, 255)) { // if white then paste
				resData[i * mask.cols + j] = ImgData[i * mask.cols + j];
			}
			else { // 검은색 부분은 300으로 채우기
				resData[i * mask.cols + j] = cv::Vec3i(300, 300, 300);
			}
		}
	}
}
/* Cut the border of the image and fill it with background and save it in savefile
des is backGroun Img, videofile is original videofile,  savefile Is the name of the file to be saved after cutting the border */
void sub_Background::capture_ROI(cv::Mat &des, char *videoFile, char *saveFile)
{
	cv::VideoCapture capture(videoFile); 
	cv::VideoWriter vw;

	vw = cv::VideoWriter(saveFile, CV_FOURCC('D', 'I', 'V', 'X'), capture.get(CV_CAP_PROP_FPS), cv::Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH), (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT)), true);

	cv::Mat Img = cv::Mat(capture.get(CV_CAP_PROP_FRAME_HEIGHT), capture.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Mat desMask = cv::Mat(Img.rows, Img.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Mat resMask = cv::Mat(Img.rows, Img.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Mat res = cv::Mat(Img.rows, Img.cols, CV_8UC3, cv::Scalar(0, 0, 0));


	cv::Point pt(Img.cols * (1 - cutRate) / 2, Img.rows * (1 - cutRate) / 2);
	cv::Rect rect(pt, cv::Size(Img.cols * cutRate, Img.rows * cutRate));
	cv::rectangle(desMask, rect, cv::Scalar(255, 255, 255), cv::FILLED);
	cv::rectangle(resMask, rect, cv::Scalar(255, 255, 255), cv::FILLED);
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