
#include "dataset.h"
#include "parse_config.h"

using namespace std;
using namespace cv;

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
