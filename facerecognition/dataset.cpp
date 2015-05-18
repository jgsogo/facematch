
#include "dataset.h"
#include "parse_config.h"

using namespace std;
using namespace cv;

int read_labels_dataset(const string& config_file, map<int, string>& labels) {
	auto cfg = parse_config(config_file);

    // Read images and labels
    ifstream file(cfg["labels"].c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, token, label;
	size_t id;
	while (getline(file, line)) {
		if (line.substr(0, 1) == "#") { continue; }
		stringstream liness(line);
		getline(liness, token, ';'); id = atoi(token.c_str());
		getline(liness, label);
		labels.insert(make_pair(id, label));
    }

}

int read_train_dataset(const string& config_file, vector<Mat>& images, vector<int>& labels) {
	auto train_cfg = parse_config(config_file);
	auto images_file = train_cfg["dataset"];
    auto faces_file = train_cfg["faces"];

    // Read images and labels
    ifstream file(images_file.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, token;
	size_t id, label;
	map<size_t, size_t> images_label;
	while (getline(file, line)) {
		if (line.substr(0, 1) == "#") { continue; }
		stringstream liness(line);
		getline(liness, token, ';'); id = atoi(token.c_str());
		getline(liness, token, ';');
		getline(liness, token); label = atoi(token.c_str());
		images_label.insert(make_pair(id, label));
    }

    // Read faces
    ifstream faces(faces_file.c_str(), ifstream::in);
    if (!faces) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	vector<Mat> tmp_images;
	string path;
	size_t image_id;
	Size min_size = Size(numeric_limits<Size::value_type>::max(), numeric_limits<Size::value_type>::max());
	while(getline(faces, line)) {
        if (line.substr(0, 1) == "#") {continue; }
        stringstream liness(line);
		getline(liness, token, ';'); id = atoi(token.c_str());
		getline(liness, path, ';');
		getline(liness, token, ';'); image_id = atoi(token.c_str());
		getline(liness, token);
        auto it = tmp_images.insert(tmp_images.end(), imread(path, 0));
        labels.push_back(images_label[image_id]);
        min_size.width = min((*it).cols, min_size.width);
        min_size.height = min((*it).rows, min_size.height);
	}

	// All images must have the same size
    for (auto it = tmp_images.begin(); it!=tmp_images.end(); ++it) {
        Mat res;
        resize(*it, res, min_size);
        images.push_back(res);
    }
}


int get_image_names(const string& config_file, map<int, string>& face_correspondence) {
    // Find correspondences between face_id and image_name
    auto cfg = parse_config(config_file);
	auto images_file = cfg["dataset"];
    auto faces_file = cfg["faces"];

    // Read images and labels
    ifstream file(images_file.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, token, path;
	size_t id, label;
	map<size_t, string> images_name;
	while (getline(file, line)) {
		if (line.substr(0, 1) == "#") { continue; }
		stringstream liness(line);
		getline(liness, token, ';'); id = atoi(token.c_str());
		getline(liness, path, ';');
		getline(liness, token);
        auto found = path.find_last_of("/\\");
        auto image_name = path.substr(found);
		images_name.insert(make_pair(id, image_name));
    }

    // Read faces and assign
    ifstream faces(faces_file.c_str(), ifstream::in);
	if (!faces) {
		string error_message = "No valid input file was given, please check the given filename: '" + faces_file + "'.";
		CV_Error(CV_StsBadArg, error_message);
	}
	size_t image_id;
	while (getline(faces, line)) {
		if (line.substr(0, 1) == "#") { continue; }
		stringstream liness(line);
		getline(liness, token, ';'); id = atoi(token.c_str());
		getline(liness, token, ';');
		getline(liness, token, ';'); image_id =  atoi(token.c_str());
		getline(liness, token);
		face_correspondence.insert(make_pair(id, images_name[image_id]));
    }
}
