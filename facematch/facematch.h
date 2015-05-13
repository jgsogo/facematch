
#pragma once

#include <string>
#include <opencv2/opencv.hpp>


std::string detectFaces(const std::string& csv_dataset, const cv::Size& min_face_size, const std::string& working_dir, std::ostream& os = std::cout);
double get_distance(const cv::Mat& lhs, const cv::Mat& rhs, bool resize = false);
void compute_distances(const std::string& csv_images, const std::size_t& limit_images = std::numeric_limits<std::size_t>::max(), std::ostream& os = std::cout);