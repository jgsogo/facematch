
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

int main(int argc, char** argv ) {
    if ( argc != 2 ) {
		cout << "usage: " << argv[0] << " <csv.ext>" << endl;
        exit(1);
    }
	// Read CSV file
	map<size_t, Mat> images;
	vector<int> labels;
	try {
		cout << "Reading dataset... ";
		read_csv(argv[1], images, labels, ';', 150);
		cout << images.size() << " images read" << endl;
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << argv[1] << "\". Reason: " << e.msg << endl;
		exit(1);
	}
	if (images.size() <= 1) {
		string error_message = "Not enough images to work. Please add more images to your dataset!";
		CV_Error(CV_StsError, error_message);
	}

	// Work on each image to get faces and eyes
	map<size_t, vector<Face>> faces;
	int common_size = numeric_limits<int>::max();
	cv::Size min_size(30, 30);
	for_each(images.begin(), images.end(), [&faces, &common_size, &min_size](const pair<size_t, Mat>& item) {
		auto faces_aux = FaceDetection::detectFaces(item.second, min_size, true);
		for (auto& face : faces_aux) {
			if (face.hasEyes()) {
				common_size = (min)(common_size, face.getImage().size().width);
				auto it = faces.insert(make_pair(item.first, vector<Face>()));	
				it.first->second.push_back(face);
			}
		}
	});

	std::for_each(faces.begin(), faces.end(), [](const pair<size_t, vector<Face>>& item) {
		for (auto& face : item.second) {
			imshow("Image", face.crop(50, true));
			waitKey(0);
		}
	});


    
    //  * work on each image to get faces
    //  * crop and rescale

    // Unsupervised classification
    //  * apply unsupervised classification: kmeans
    //  * store each cluster into a different folder and name the photos with numbers
    //  proportional to the distance to the centroid.
    waitKey(0);

    return 0;
}
