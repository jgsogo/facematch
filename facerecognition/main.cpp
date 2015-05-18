
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
    Size min_size(images[0].cols, images[0].rows);

    // Models for recognizing
    Ptr<FaceRecognizer> model_eigen = createEigenFaceRecognizer();
    Ptr<FaceRecognizer> model_fisher = createFisherFaceRecognizer();
    Ptr<FaceRecognizer> model_lbph = createLBPHFaceRecognizer();

    model_eigen->train(images, labels);
    model_fisher->train(images, labels);
    model_lbph->train(images, labels);


    // Do classification
	auto config_file = string(argv[2]);
    auto classify_cfg = parse_config(config_file);
	auto found = config_file.find_last_of("/\\");
	auto working_dir = config_file.substr(0, found);

    // Read faces
    ifstream faces(classify_cfg["faces"].c_str(), ifstream::in);
    if (!faces) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string path, line, token;
	size_t image_id, id;
	map<int, vector<pair<int, double>>> predictions_eigen;
	map<int, vector<pair<int, double>>> predictions_fisher;
	map<int, vector<pair<int, double>>> predictions_lbph;
	size_t i = 0;
	while(getline(faces, line)) {
        if (line.substr(0, 1) == "#") {continue; }
        stringstream liness(line);
		getline(liness, token, ';'); id = atoi(token.c_str());
		getline(liness, path, ';');
		getline(liness, token, ';'); image_id = atoi(token.c_str());
		getline(liness, token);

        Mat res = imread(path, 0);
        Mat img; resize(res, img, min_size);

        // Predict!
        int predictedLabel = -1;
        double confidence = 0.0;
        model_eigen->predict(img, predictedLabel, confidence);
		predictions_eigen[predictedLabel].push_back(make_pair(image_id, confidence));

        confidence = 0.0; predictedLabel = -1;
        model_fisher->predict(img, predictedLabel, confidence);
		predictions_fisher[predictedLabel].push_back(make_pair(image_id, confidence));

        confidence = 0.0; predictedLabel = -1;
        model_lbph->predict(img, predictedLabel, confidence);
		predictions_lbph[predictedLabel].push_back(make_pair(image_id, confidence));
		i++;
	}
    cout << "Classified " << i << " faces" << endl;

    cout << endl << "Eigenfaces" << endl;
    for (auto it = predictions_eigen.begin(); it!=predictions_eigen.end(); ++it) {
        cout << "Label " << it->first << " got " << it->second.size() << " images [" << it->second.size()/float(i)*100 << " %]" << endl;
    }

    cout << endl << "Fisherfaces" << endl;
    for (auto it = predictions_fisher.begin(); it!=predictions_fisher.end(); ++it) {
        cout << "Label " << it->first << " got " << it->second.size() << " images [" << it->second.size()/float(i)*100 << " %]" << endl;
    }

    cout << endl << "Local Binary Pattern Histogram faces" << endl;
    for (auto it = predictions_lbph.begin(); it!=predictions_lbph.end(); ++it) {
        cout << "Label " << it->first << " got " << it->second.size() << " images [" << it->second.size()/float(i)*100 << " %]" << endl;
    }

    waitKey(0);

    return 0;
}
