
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

int read_train_dataset(const std::string& config_file, std::vector<cv::Mat>& images, std::vector<int>& labels);
int read_labels_dataset(const std::string& config_file, std::map<int, std::string>& labels);

int get_image_names(const std::string& config_file, std::map<int, std::string>& face_correspondence);
