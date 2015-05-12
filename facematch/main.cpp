
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

static void read_csv(const string& filename, map<size_t, FaceDetection>& images, vector<int>& labels, char separator = ';', const size_t& max_items = numeric_limits<size_t>::max()) {
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
	map<size_t, FaceDetection> images;
	vector<int> labels;
	try {
		cout << "Reading dataset... ";
		read_csv(argv[1], images, labels, ';', 10);
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
	std::for_each(images.begin(), images.end(), [](pair<const size_t, FaceDetection>& item) {
		item.second.detect();
		//item.second.show();
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
