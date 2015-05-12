
#include "FaceDetection.h"

using namespace cv;
using namespace std;

struct FaceDetection::ClassifierData {
	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;
	ClassifierData() {
		auto hret = face_cascade.load("./data/etc/haarcascades/haarcascade_frontalface_alt.xml");
		eyes_cascade.load("./data/etc/haarcascades/haarcascade_eye_tree_eyeglasses.xml");
		};
};

void FaceDetection::initClassfier() {
	if (!classifier) {
		classifier = new ClassifierData();
	}
}

FaceDetection::ClassifierData* FaceDetection::classifier = 0;


vector<Face> FaceDetection::detectFaces(const Mat& image, const Size& min_face_size, bool detect_eyes, const Size& min_eye_size) {
	FaceDetection::initClassfier();
	vector<Face> faces;
	vector<Rect> aux;
	Mat frame_gray;
	cvtColor(image, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	classifier->face_cascade.detectMultiScale(frame_gray, aux, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, min_face_size);
	for (auto& rect : aux) {
		auto it = faces.insert(faces.end(), Face(image(rect)));
		if (detect_eyes) {
			it->detectEyes(min_eye_size);
		}
	}
	return faces;
}

vector<Rect> FaceDetection::detectEyes(const Mat& image, const Size& min_size, const Rect& face) {
	FaceDetection::initClassfier();
	vector<Rect> eyes;
	Mat faceROI = face.width ? image(face) : image;
	cvtColor(faceROI, faceROI, CV_BGR2GRAY);
	equalizeHist(faceROI, faceROI);
	classifier->eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, min_size);
	return eyes;
}
