
#pragma once

#include <string>
#include <opencv2/opencv.hpp>


void detectFaces(const std::string& csv_dataset, const cv::Size& min_face_size, const std::string& faces_filename, std::ostream& os = std::cout);
double get_distance(const cv::Mat& lhs, const cv::Mat& rhs, bool resize = false);
void compute_distances(const std::string& csv_images, const std::string& distances_filename, std::ostream& os = std::cout);

