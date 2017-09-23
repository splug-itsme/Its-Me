#include "analyze_Emtoion.h"

string emotionFileName1 = "./data/neutral_vs_rest.dat";
string emotionFileName2 = "./data/happy_vs_rest.dat";
string emotionFileName3 = "./data/sad_vs_rest.dat";
string emotionFileName4 = "./data/surprise_vs_rest.dat";
string shapeFileName = "./data/shape_predictor_68_face_landmarks.dat";
shape_predictor sp;
pfunct_type ep1;
pfunct_type ep2;
pfunct_type ep3;
pfunct_type ep4;
int faceNumber = 0;
frontal_face_detector face_detector = get_frontal_face_detector();

analyze_Emtoion::analyze_Emtoion()
{

}

analyze_Emtoion::analyze_Emtoion(std::vector<cv::Mat> face) // 감정분석
{
	std::vector <double> happySize;


	deserialize(shapeFileName) >> sp;
	deserialize(emotionFileName1) >> ep1;
	deserialize(emotionFileName2) >> ep2;
	deserialize(emotionFileName3) >> ep3;
	deserialize(emotionFileName4) >> ep4;


	cout << "\n\nProgram Started\n\n";
	for (int i = 0; i < face.size(); i++) { // face에는 감정분석 비교에 들어갈 사진들 = 같은 인물의 사진
		int noOfFaces = 0;
		faceNumber = 0;
		std::vector<sample_type> samples;

		samples = getAllAttributes(face[i]);

		std::vector<double> prob;
		if (samples.size() == 0) { // 얼굴 못잡으면 넘어감
			happySize.push_back(0);
			continue;
		}
		prob = svm_Multiclass(samples[0]);
		prob = cal_Probablity(prob); // data를 가지고 각 감정의 가능성 분석
		cout << "probablity that face " << " is Neutral  :" << prob[0] << endl;
		cout << "probablity that face " << " is Happy    :" << prob[1] << endl;
		cout << "probablity that face " << " is Sad      :" << prob[2] << endl;
		cout << "probablity that face " << " is Surprise :" << prob[3] << "\n\n\n";
		happySize.push_back(prob[1]);


		//cout << "\n\nPress Enter to delete all Photos.............";
		//cin.ignore();
	}
	faceN = find_Num(happySize);
}


std::vector<sample_type> analyze_Emtoion::getAllAttributes(cv::Mat &face) // 사진하나에 대해서 수행한다.
{
	int i, j, k;
	std::vector<sample_type> samples;
	sample_type sample;
	stringstream s;

	faceNumber = 1;
	for (i = 0; i < faceNumber; i++) 
	{
		array2d<rgb_pixel> img;
		dlib::assign_image(img, dlib::cv_image<bgr_pixel>(face)); // Mat 형식 array<rgb_pixel>로 포맷 변환

		pyramid_up(img); // 사진 확대

		std::vector<dlib::rectangle> faceRectangles = face_detector(img);
		if (faceRectangles.size() == 0)
			continue;
		// 여기서 얼굴 사각형을 못잡으면 나가도록 구현
		full_object_detection feature = sp(img, faceRectangles[0]);
		int l = 0;
		for (int j = 0; j < 68; j++)
			for (int k = 0; k < j; k++, l++)
			{
				sample(l) = length(feature.part(j), feature.part(k));
				l++;
				sample(l) = slope(feature.part(j), feature.part(k));

			}
		samples.push_back(sample);

	}
	return samples;
}

double analyze_Emtoion::length(point a, point b)
{
	int x1, y1, x2, y2;
	double dist;
	x1 = a.x();
	y1 = a.y();
	x2 = b.x();
	y2 = b.y();

	dist = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	dist = sqrt(dist);
	return dist;
}

double analyze_Emtoion::slope(point a, point b)
{
	int x1, y1, x2, y2;

	x1 = a.x();
	y1 = a.y();
	x2 = b.x();
	y2 = b.y();
	if ((x1 - x2) == 0)
		if ((y1 - y2) > 0)
			return (M_PI / 2);
		else
			return (-M_PI / 2);
	else
		return atan(double(y1 - y2)) / (x1 - x2);
}

void analyze_Emtoion::remove_Photos()
{
	int i;
	stringstream s;

	for (i = 0; i < faceNumber; i++)
	{
		s.str("");
		s << "face" << i << ".jpg";
		remove(s.str().c_str());
	}
}


std::vector<double> analyze_Emtoion::svm_Multiclass(sample_type sample)
{
	std::vector<double> probs;
	probs.push_back(ep1(sample));
	probs.push_back(ep2(sample));
	probs.push_back(ep3(sample));
	probs.push_back(ep4(sample));

	return probs;
}

std::vector<double> analyze_Emtoion::cal_Probablity(std::vector<double> P)
{
	std::vector<double> EmoProb(4);
	double sum = P[0] + P[1] + P[2] + P[3];
	EmoProb[0] = P[0] / sum;
	EmoProb[1] = P[1] / sum;
	EmoProb[2] = P[2] / sum;
	EmoProb[3] = P[3] / sum;
	return EmoProb;
}

int analyze_Emtoion::find_Num(std::vector<double> happySize)
{
	if (happySize.size() == 0) // 못찾을 경우
		return -1;
	double maxX = *max_element(happySize.begin(), happySize.end());
	if (maxX < 0.01)
		return -1;
	for (int i = 0; i < happySize.size(); i++)
	{
		if (maxX == happySize[i])
			return i;
	}
	return -1; // 실패
}

