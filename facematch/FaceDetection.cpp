
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


FaceDetection::ClassifierData* FaceDetection::classifier = 0;


FaceDetection::FaceDetection(const Mat& img) : image(img) {
	if (!classifier) {
		classifier = new ClassifierData();
	}
}

void FaceDetection::detect() {
	auto faces = detectFaces(image);
	for (auto& face: faces) {
		auto eyes = detectEyes(image, face);
		faces_data.push_back(std::make_pair(face, eyes));
	}
}

vector<Rect> FaceDetection::detectFaces(const Mat& image) {
	vector<Rect> faces;
	Mat frame_gray;
	cvtColor(image, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	classifier->face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	return faces;
}

vector<Rect> FaceDetection::detectEyes(const Mat& image, const Rect& face) {
	vector<Rect> eyes;
	Mat faceROI = image(face);
	cvtColor(faceROI, faceROI, CV_BGR2GRAY);
	equalizeHist(faceROI, faceROI);
	classifier->eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	return eyes;
}

void FaceDetection::show() const {
	for (auto& face : faces_data) {
		Point center(face.first.x + face.first.width*0.5, face.first.y + face.first.height*0.5);
		ellipse(image, center, Size(face.first.width*0.5, face.first.height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
		for (auto& eye : face.second) {
			Point center(face.first.x + eye.x + eye.width*0.5, face.first.y + eye.y + eye.height);
			int radius = cvRound((eye.width + eye.height)*0.25);
			circle(image, center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}
	}
	imshow("Image", image);
}
