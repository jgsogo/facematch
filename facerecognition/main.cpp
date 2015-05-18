
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <functional>
#include <iterator>
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
    cout << "\t Train classifiers" << endl;
    Ptr<FaceRecognizer> model_eigen = createEigenFaceRecognizer();
    Ptr<FaceRecognizer> model_fisher = createFisherFaceRecognizer();
    Ptr<FaceRecognizer> model_lbph = createLBPHFaceRecognizer();

    model_eigen->train(images, labels);
    model_fisher->train(images, labels);
    model_lbph->train(images, labels);


    // Do classification
    cout << endl << "Starting classification:" << endl;
	auto config_file = string(argv[2]);
    auto classify_cfg = parse_config(config_file);
    map<int, string> image_names;
    get_image_names(config_file, image_names);
    cout << "\t ...working on " << image_names.size() << " faces." << endl;

    // Read faces
    ifstream faces(classify_cfg["faces"].c_str(), ifstream::in);
    if (!faces) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string path, line, token;
	size_t face_id, id;
	map<int, vector<pair<int, double>>> predictions_eigen; // <label, <image_id, confidence>>
	map<int, vector<pair<int, double>>> predictions_fisher; // <label, <image_id, confidence>>
	map<int, vector<pair<int, double>>> predictions_lbph; // <label, <image_id, confidence>>
	map<int, vector<pair<int, double>>> predictions; // <face_id, <label, confidence>>
	double max_eigen=0.0, max_fisher=0.0, max_lbph=0.0;
	size_t i = 0;
	while(getline(faces, line)) {
        if (line.substr(0, 1) == "#") {continue; }
        stringstream liness(line);
		getline(liness, token, ';'); id = atoi(token.c_str());
		getline(liness, path, ';');
		getline(liness, token, ';');// image_id = atoi(token.c_str());
		getline(liness, token);

        Mat res = imread(path, 0);
        Mat img; resize(res, img, min_size);

        // Predict!
        auto it_face = predictions.insert(make_pair(id, vector<pair<int,double>>()));
        int predictedLabel = -1;
        double confidence = 0.0;
        model_eigen->predict(img, predictedLabel, confidence);
		predictions_eigen[predictedLabel].push_back(make_pair(id, confidence));
        it_face.first->second.push_back(make_pair(predictedLabel, confidence));
        max_eigen = max(max_eigen, confidence);

        confidence = 0.0; predictedLabel = -1;
        model_fisher->predict(img, predictedLabel, confidence);
		predictions_fisher[predictedLabel].push_back(make_pair(id, confidence));
		it_face.first->second.push_back(make_pair(predictedLabel, confidence));
		max_fisher = max(max_fisher, confidence);

        confidence = 0.0; predictedLabel = -1;
        model_lbph->predict(img, predictedLabel, confidence);
		predictions_lbph[predictedLabel].push_back(make_pair(id, confidence));
		it_face.first->second.push_back(make_pair(predictedLabel, confidence));
		max_lbph = max(max_lbph, confidence);
		i++;

		if (i % 50 == 0) {
            cout << i << " faces classified" << endl;
			}
	}
	faces.close();
    cout << "Classified " << i << " faces" << endl;

    map<int, string> labels_str;
    read_labels_dataset(argv[1], labels_str);

    cout << endl << "Eigenfaces" << endl;
    for (auto it = predictions_eigen.begin(); it!=predictions_eigen.end(); ++it) {
        cout << "Label '" << labels_str[it->first] << "' got " << it->second.size() << " images [" << it->second.size()/float(i)*100 << " %]" << endl;
    }

    cout << endl << "Fisherfaces" << endl;
    for (auto it = predictions_fisher.begin(); it!=predictions_fisher.end(); ++it) {
        cout << "Label '" << labels_str[it->first] << "' got " << it->second.size() << " images [" << it->second.size()/float(i)*100 << " %]" << endl;
    }

    cout << endl << "Local Binary Pattern Histogram faces" << endl;
    for (auto it = predictions_lbph.begin(); it!=predictions_lbph.end(); ++it) {
        cout << "Label '" << labels_str[it->first] << "' got " << it->second.size() << " images [" << it->second.size()/float(i)*100 << " %]" << endl;
    }

    // Check confidence
    auto threshold = 0.66;
    cout << endl << "Confidence over '" << threshold << "':" << endl;
    vector<size_t> votes(labels_str.size(), 0);
    for (auto it = predictions.begin(); it!=predictions.end(); ++it) {
        vector<double> confidence(labels_str.size(), 0);
        confidence[it->second[0].first] += it->second[0].second/max_eigen;
        confidence[it->second[1].first] += it->second[1].second/max_eigen;
        confidence[it->second[2].first] += it->second[2].second/max_eigen;
        auto it_max = max_element(confidence.begin(), confidence.end());
        if ((*it_max) >= threshold) {
            cout << it->first << ": " << image_names[it->first] << " matched as '";
            cout << labels_str[distance(confidence.begin(), it_max)] << "'" << endl;
            votes[distance(confidence.begin(), it_max)] += 1;
        }
    }

    // Final results
    cout << endl << "Final count of votes:" << endl;
    for (auto i = 1; i<votes.size(); ++i) {
        cout << "\t " << labels_str[i] << ": " << votes[i] << " votes." << endl;
    }
    /*
    cout << endl << "Mismatched outputs:" << endl;
    for (auto it = predictions.begin(); it!=predictions.end(); ++it) {
        auto eigen_label = it->second[0].first;
        auto fisher_label = it->second[1].first;
        auto lbph_label = it->second[2].first;
        if (eigen_label != fisher_label || eigen_label != lbph_label || fisher_label != lbph_label) {
            cout << it->first << ": " << image_names[it->first] << endl;
            cout << "\t Eigenfaces:  conf '" << it->second[0].second/max_eigen << "' labeled as '" << labels_str[eigen_label] << "'." << endl;
            cout << "\t Fisherfaces: conf '" << it->second[1].second/max_fisher << "' labeled as '" << labels_str[fisher_label] << "'." << endl;
            cout << "\t LBPH faces:  conf '" << it->second[2].second/max_lbph << "' labeled as '" << labels_str[lbph_label] << "'." << endl;
        }
    }
    */
    /*
    cout << endl << "Some example outputs:" << endl;
    for (auto it = predictions.begin(); it!=predictions.end(); ++it) {
        cout << "Face " << it->first << endl;
        cout << "\t conf '" << it->second[0].second/max_eigen << "' labeled as '" << labels_str[it->second[0].first] << "'." << endl;
        cout << "\t conf '" << it->second[1].second/max_fisher << "' labeled as '" << labels_str[it->second[0].first] << "'." << endl;
        cout << "\t conf '" << it->second[2].second/max_lbph << "' labeled as '" << labels_str[it->second[0].first] << "'." << endl;
    }
    */

    waitKey(0);

    return 0;
}
