
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <functional>
#include <opencv2/opencv.hpp>

#include "facematch.h"
#include "FaceDetection.h"

using namespace cv;
using namespace std;

static void read_csv(const string& filename, map<size_t, Mat>& images, vector<int>& labels, char separator = ';', const size_t& max_items = numeric_limits<size_t>::max()) {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	auto n_items = max_items;
	string line, path, classlabel, token;
	std::size_t id;
	while (getline(file, line) && --n_items) {
		if (line.substr(0, 1) == "#") { ++n_items;  continue; }
		stringstream liness(line);
		getline(liness, token, separator); id = atoi(token.c_str());
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			images.insert(make_pair(id, imread(path)));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
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
	cout << "Detecting faces in dataset '" << cfg["dataset"] << "'..." << endl;
	//auto faces_csv = detectFaces(cfg["dataset"], cv::Size(faces_min_size, faces_min_size), working_dir);
	
	// Compute distances
	//cout << "Computing distances in dataset '" << faces_csv << "'..." << endl;
	compute_distances(cfg["faces_csv"], 10);

    // Unsupervised classification
    //  * apply unsupervised classification: kmeans
    //  * store each cluster into a different folder and name the photos with numbers
    //  proportional to the distance to the centroid.
    waitKey(0);

    return 0;
}
