
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <functional>
#include <opencv2/opencv.hpp>

#include "parse_config.h"
#include "dataset.h"


using namespace cv;
using namespace std;

bool is_file_exist(const string& filename) {
    std::ifstream infile(filename);
    return infile.good();
}


int main(int argc, char** argv ) {
    if ( argc != 3 ) {
		cout << "usage: " << argv[0] << " <train.cfg> <classify.cfg>" << endl;
        exit(1);
    }
	// Parse training images
	cout << "Read training data:" << endl;
	vector<Mat> images;
	vector<int> labels;
	read_train_dataset(argv[1], images, labels);
	cout << "\t Got " << images.size() << " images for training" << endl;

	auto config_file = string(argv[2]);
    auto classify_cfg = parse_config(config_file);
	auto found = config_file.find_last_of("/\\");
	auto working_dir = config_file.substr(0, found);


    waitKey(0);

    return 0;
}
