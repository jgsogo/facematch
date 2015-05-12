
#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Face.h"

class FaceDetection {
	public:
		static cv::Mat cropFace(const cv::Mat& image, const cv::Rect& rect);
		static std::vector<Face> detectFaces(const cv::Mat& image, bool detect_eyes);
		static std::vector<cv::Rect> detectEyes(const cv::Mat& image, const cv::Rect& face = cv::Rect());

		static void initClassfier();
	protected:
		struct ClassifierData;
		static ClassifierData* classifier;
};