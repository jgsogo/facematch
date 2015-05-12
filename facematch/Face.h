
#pragma once
#include <opencv2/opencv.hpp>

class Face {
	public:
		Face(const cv::Mat& img);
		bool detectEyes();
		bool hasEyes() const;
		const cv::Mat& getImage() const { return image; };
		const cv::Rect& getEyeLeft() const { return eye_left; };
		const cv::Rect& getEyeRight() const { return eye_right; };
		cv::Mat crop(const float& radius, bool align_eyes);
	protected:
		cv::Mat image;
		cv::Rect eye_left, eye_right;
	};


void show(const Face&, bool show_eyes);