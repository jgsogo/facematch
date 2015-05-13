
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <functional>
#include <opencv2/opencv.hpp>

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

	// Read CSV file
	map<size_t, Mat> images;
	vector<int> labels;
	cout << "Reading dataset '" << cfg["id"] << "'... ";
	read_csv(cfg["dataset"], images, labels, ';');
	cout << images.size() << " images read." << endl;

	// Work on each image to get faces and eyes
	cout << "Looking for faces in images... " << endl;
	map<size_t, vector<Face>> faces;
	cv::Size min_size(faces_min_size, faces_min_size);
	ofstream faces_file(working_dir + "/faces.csv", ofstream::out);
	faces_file << "# id; path; image_id; eyes\n";
	auto i = 0;
	for_each(images.begin(), images.end(), [&faces, &min_size, &working_dir, &faces_file, &i](const pair<size_t, Mat>& item) {
        try {
            auto faces_aux = FaceDetection::detectFaces(item.second, min_size, true);
			auto it = faces.insert(make_pair(item.first, vector<Face>()));
            for (auto& face : faces_aux) {
				std::stringstream ss; ss << working_dir << "/image_" << item.first << "_face_" << i << ".jpg";
				faces_file << i << ";" << ss.str() << ";" << item.first << ";" << face.hasEyes() << "\n";
				Mat image = face.crop(0, true);
				imwrite(ss.str(), image);
				it.first->second.push_back(image);
				i++;
				if (item.first % 20 == 0) {
					cout << item.first << " images processed. Found " << i << " faces." << endl;
				}
            }
        }
        catch(Exception& e) {
            cout << "cv::Exception: " << e.what() << endl;
        }
        catch(exception& e) {
            cout << "std::Exception: " << e.what() << endl;
        }
	});

    // Unsupervised classification
    //  * apply unsupervised classification: kmeans
    //  * store each cluster into a different folder and name the photos with numbers
    //  proportional to the distance to the centroid.
    waitKey(0);

    return 0;
}
