
#pragma once

#include <string>
#include <iostream>
#include <vector>

// auto-tuning spectral clustering and kmeans spectral clustering
std::vector<std::vector<int>> compute_spectral(const std::string& distances_csv, const std::string& spectral_filename, std::ostream& os = std::cout);
std::vector<std::vector<int>> compute_kmeans(const std::string& distances_csv, const size_t& k, const std::string& spectral_filename, std::ostream& os = std::cout);
