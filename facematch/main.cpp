
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <functional>
#include <opencv2/opencv.hpp>

#include "facematch.h"
#include "clustering.h"


using namespace cv;
using namespace std;

bool is_file_exist(const string& filename) {
    std::ifstream infile(filename);
    return infile.good();
}

map<string, string> parse_config(const string& filename) {
	ifstream cfg(filename, ifstream::in);
	if (!cfg) {
		cout << "cannot read config file: '" << filename << "'" << endl;
		exit(1);
	}
	map<string, string> ret;
	string line;
	while (getline(cfg, line)) {
		istringstream is_line(line);
		string key;
		if (getline(is_line, key, '=')) {
			string value;
			if (getline(is_line, value)) {
				ret[key] = value;
			}
		}
	}
	return ret;
}

int main(int argc, char** argv ) {
    if ( argc != 2 ) {
		cout << "usage: " << argv[0] << " <file.cfg>" << endl;
        exit(1);
    }
	// Parse config file
	auto config_file = string(argv[1]);
	auto cfg = parse_config(config_file);
	auto found = config_file.find_last_of("/\\");
	auto working_dir = config_file.substr(0, found);
	auto faces_min_size = (cfg.find("faces_min_size") != cfg.end()) ? atoi(cfg["faces_min_size"].c_str()) : 30;

	// Find faces in images
	string faces_csv = working_dir + "/faces.csv";
	if (!is_file_exist(faces_csv)) {
        cout << "Detecting faces in dataset '" << cfg["dataset"] << "'..." << endl;
        detectFaces(cfg["dataset"], cv::Size(faces_min_size, faces_min_size), faces_csv);
    }
    else {
        cout << "Faces already computed in '" << faces_csv << "'." << endl;
    }

	// Compute distances
	string distances_csv = working_dir + "/distance.csv";
	if (!is_file_exist(distances_csv)) {
        cout << "Computing distances in dataset '" << faces_csv << "'..." << endl;
        compute_distances(faces_csv, distances_csv);
	}
	else {
        cout << "Distances already computed in '" << distances_csv << "'." << endl;
	}

    // Make spectral clustering
    string spectral_clustering = working_dir + "/spectral_clustering.csv";
    if (!is_file_exist(spectral_clustering)) {
        cout << "Computing spectral clustering for dataset at '" << distances_csv << "'" << endl;
        auto cs = compute_spectral(distances_csv, spectral_clustering);
    }
    else {
        cout << "Spectral clustering already computed in '" << spectral_clustering << "'." << endl;
    }

    // KMeans spectral clustering
    string kmeans_clustering = working_dir + "/kmeans_clustering.csv";
    if (!is_file_exist(kmeans_clustering)) {
        cout << "Computing kmeans clustering for dataset at '" << kmeans_clustering << "'" << endl;
        auto k = 20;
        auto cs = compute_kmeans(distances_csv, k, kmeans_clustering);
    }
    else {
        cout << "Kmeans clustering already computed in '" << kmeans_clustering << "'." << endl;
    }

    // Unsupervised classification
    //  * apply unsupervised classification: kmeans
    //  * store each cluster into a different folder and name the photos with numbers
    //  proportional to the distance to the centroid.
    waitKey(0);

    return 0;
}
