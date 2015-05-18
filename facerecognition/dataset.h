
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

int read_train_dataset(const std::string& config_file, std::vector<cv::Mat>& images, std::vector<int>& labels);
