
#include "Face.h"
#include "FaceDetection.h"

#include <iostream>

using namespace cv;
using namespace std;

Face::Face(const cv::Mat& img) : image(img) {}

bool Face::detectEyes(const cv::Size& min_eye_size) {
	auto eyes = FaceDetection::detectEyes(image, min_eye_size);
	if (eyes.size() == 2) {
		eye_left = (eyes[0].x <= eyes[1].x) ? eyes[0] : eyes[1];
		eye_right = (eyes[0].x > eyes[1].x) ? eyes[0] : eyes[1];
		return true;
	}
	return false;
}

bool Face::hasEyes() const {
	return (eye_left.width != 0) && (eye_right.width != 0);
}

Mat Face::crop(const float& out_radius, bool align_eyes) const {
	// Crop to circle
	Point center(image.cols / 2., image.rows / 2.);
	int radius = (max)(image.cols, image.rows)/2.;
	Mat im2(image.rows, image.cols, image.type(), Scalar(0, 0, 0));
	circle(im2, center, radius, Scalar(255, 255, 255), -1, 8);

	Mat res;
	bitwise_and(image, im2, res);

	if (align_eyes) {
		auto angle = fastAtan2(eye_right.y - eye_left.y, eye_right.x - eye_left.x);
		Mat rot_mat = getRotationMatrix2D(center, angle, 1.0);
		warpAffine(res, res, rot_mat, res.size());		
	}

	// Scale
	Mat ret;
	resize(res, ret, Size(out_radius * 2, out_radius * 2));

	return ret;
}

void show(const Face& face, bool show_eyes) {
	Mat im = face.getImage();
	if (face.hasEyes() && show_eyes) {
		auto eye1 = face.getEyeLeft();
		Point center(eye1.x + eye1.width*0.5, eye1.y + eye1.height*0.5);
		int radius = cvRound((eye1.width + eye1.height)*0.25);
		circle(im, center, radius, Scalar(255, 0, 0), 4, 8, 0);

		auto eye2 = face.getEyeRight();
		center = Point(eye2.x + eye2.width*0.5, eye2.y + eye2.height*0.5);
		radius = cvRound((eye2.width + eye2.height)*0.25);
		circle(im, center, radius, Scalar(255, 0, 0), 4, 8, 0);
	}
	imshow("Image", im);
}