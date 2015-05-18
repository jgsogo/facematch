
#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Face.h"

class FaceDetection {
	public:
		static cv::Mat cropFace(const cv::Mat& image, const cv::Rect& rect);
		static std::vector<Face> detectFaces(const cv::Mat& image, const cv::Size& min_face_size = cv::Size(30, 30), bool detect_eyes = false, const cv::Size& min_eye_size = cv::Size(5,5));
		static std::vector<cv::Rect> detectEyes(const cv::Mat& image, const cv::Size& min_size = cv::Size(10, 10), const cv::Rect& face = cv::Rect());

		static void initClassfier();
	protected:
		struct ClassifierData;
		static ClassifierData* classifier;
};
