
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

class FaceDetection {
	public:
		FaceDetection(const cv::Mat& img);

		void detect();
		static std::vector<cv::Rect> detectFaces(const cv::Mat& image);
		static std::vector<cv::Rect> detectEyes(const cv::Mat& image, const cv::Rect& face);

		std::vector<std::pair<cv::Rect, std::vector<cv::Rect>>> get_faces() const { return faces_data; };
		void show() const;
	protected:
		cv::Mat image;		
		std::vector<std::pair<cv::Rect, std::vector<cv::Rect>>> faces_data;
		
		struct ClassifierData;
		static ClassifierData* classifier;
};