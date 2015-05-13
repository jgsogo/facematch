
#include "facematch.h"
#include <fstream>

#include "FaceDetection.h"

using namespace std;
using namespace cv;

void detectFaces(const string& csv_dataset, const Size& min_face_size, const std::string& faces_filename, std::ostream& os) {
	ifstream file(csv_dataset.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	auto found = faces_filename.find_last_of("/\\");
	auto working_dir = faces_filename.substr(0, found);

	string line, path, classlabel, token;
	size_t id;
	auto i = 0;
	ofstream faces_file(faces_filename);
	faces_file << "# id; path; image_id; eyes\n";
	while (getline(file, line)) {
		if (line.substr(0, 1) == "#") { continue; }
		stringstream liness(line);
		getline(liness, token, ';'); id = atoi(token.c_str());
		getline(liness, path, ';');
		getline(liness, classlabel);
		if (!path.empty()) {
			try {
				auto faces = FaceDetection::detectFaces(imread(path), min_face_size, true);
				for (auto& face : faces) {
					std::stringstream ss; ss << working_dir << "/image_" << id << "_face_" << i << ".jpg";
					faces_file << i << ";" << ss.str() << ";" << id << ";" << face.hasEyes() << "\n";
					Mat image = face.crop(0, true);
					imwrite(ss.str(), image);
					i++;
					if (id % 50 == 0) {
						os << id << " images processed. Found " << i << " faces." << endl;
					}
				}
			}
			catch (Exception& e) {
				cout << "cv::Exception: " << e.what() << endl;
			}
			catch (exception& e) {
				cout << "std::Exception: " << e.what() << endl;
			}
		}
	}
	faces_file.close();
	os << "Output DB writen in '" << faces_filename << "'" << endl;
}

double get_distance(const Mat& lhs, const Mat& rhs, bool do_resize) {
	if (lhs.rows == 0 || lhs.cols == 0 || rhs.rows == 0 || rhs.cols == 0) { // Corner case
		return numeric_limits<double>::max();
	}

	if (lhs.rows == rhs.rows && lhs.cols == rhs.cols) {
		auto error = norm(lhs, rhs, CV_L2);
		return error / static_cast<double>(lhs.rows*lhs.cols);
	}
	else if (do_resize) {
		Size minimum((min)(lhs.rows, rhs.rows), (min)(lhs.cols, rhs.cols));
		Mat lhs2; Mat rhs2;
		resize(lhs, lhs2, minimum);
		resize(rhs, rhs2, minimum);
		return get_distance(lhs2, rhs2);
	}
	else {
		return numeric_limits<double>::max();
	}
}

void compute_distances(const string& csv_images, const string& distances_filename, std::ostream& os) {
	ifstream file(csv_images.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}

	os << "Reading images..." << endl;
	vector<pair<size_t, Mat>> images;
	string line, path, token;
	size_t id;
	auto i = 0;
	while (getline(file, line)) {
		if (line.substr(0, 1) == "#") { continue; }
		stringstream liness(line);
		getline(liness, token, ';'); id = atoi(token.c_str());
		getline(liness, path, ';');
		getline(liness, token); // To endline
		if (!path.empty()) {
			images.push_back(make_pair(id, imread(path)));
			i++;
			if (i % 50 == 0) {
				os << i << " images read" << endl;
			}
		}
	}

	auto n_computations = pow(images.size()-1, 2)/2.;
	os << "Compute distances between " << images.size() << " images (" << n_computations << " computations)." << endl;
	auto found = csv_images.find_last_of("/\\");
	auto working_dir = csv_images.substr(0, found);

	ofstream distances_file(distances_filename);
	distances_file << "# id1; id2; distance\n";

	i = 0;
	for (auto im1 = images.begin(); im1 != images.end(); ++im1) {
		for (auto im2 = im1+1; im2 != images.end(); ++im2) {
			auto d = get_distance(im1->second, im2->second, true);
			distances_file << im1->first << ";" << im2->first << ";" << d << "\n";
			i++;
			if (i % 50 == 0) {
				os << i << "/" << n_computations << " distances computed" << endl;
			}
		}
	}

	distances_file.close();
	os << "Output DB writen in '" << distances_filename << "'" << endl;
}
