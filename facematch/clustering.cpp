
#include "clustering.h"

#include <tuple>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <opencv2/opencv.hpp>

#include <Eigen/Core>
#include "clustering/SpectralClustering.h"

using namespace std;

Eigen::MatrixXd build_matrix(const string& distances_csv, ostream& os) {
    ifstream file(distances_csv.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}

	os << "\t Building similarity matrix..." << endl;
	string line, token;
	size_t id1, id2, max_id;
	double value;
	vector<tuple<size_t, size_t, double>> distances;
	while (getline(file, line)) {
		if (line.substr(0, 1) == "#") { continue; }
		stringstream liness(line);
		getline(liness, token, ';'); id1 = atoi(token.c_str());
		getline(liness, token, ';'); id2 = atoi(token.c_str());
		getline(liness, token); value = atof(token.c_str());
		distances.push_back(make_tuple(id1, id2, value));
    }
    auto max = max_element(distances.begin(), distances.end(), [](const tuple<size_t, size_t, double>& a, const tuple<size_t, size_t, double>& b){
        return get<2>(a) < get<2>(b);
    });
    auto max_distance = get<2>(*max);

    auto size_it = max_element(distances.begin(), distances.end(), [](const tuple<size_t, size_t, double>& a, const tuple<size_t, size_t, double>& b){
        return get<1>(a) < get<1>(b);
    });
    auto size = get<1>(*size_it) + 1;

    //size_t size = distances.size();
    Eigen::MatrixXd m = Eigen::MatrixXd::Zero(size, size);
    for (auto it = distances.begin(); it!=distances.end(); ++it) {
        m(get<0>(*it), get<1>(*it)) = max_distance - get<2>(*it);
        m(get<1>(*it), get<0>(*it)) = max_distance - get<2>(*it);
    }

    return m;
}

void print_to_file(vector<vector<int>>& clusters, const string& filename) {
    ofstream file(filename, ofstream::out);
    for (unsigned int i=0; i < clusters.size(); i++) {
        file << "Cluster " << i << ": " << "Item ";
        copy(clusters[i].begin(), clusters[i].end(), ostream_iterator<int>(file, ", "));
        file << endl;
    }
    file.close();
}

vector<vector<int>> compute_spectral(const string& distances_csv, const string& spectral_filename, ostream& os) {
    auto m = build_matrix(distances_csv, os);

    os << "\t Do eigenvalue decomposition..." << endl;
    // do eigenvalue decomposition
    SpectralClustering* c = new SpectralClustering(m, 20);
    // auto-tuning clustering
    os << "\t Auto-tuning clustering..." << endl;
    auto clusters = c->clusterRotate();

    // output clustered items
    // items are ordered according to distance from cluster centre
    print_to_file(clusters, spectral_filename);
    return clusters;
}

vector<vector<int>> compute_kmeans(const string& distances_csv, const size_t& k, const string& kmeans_filename, ostream& os) {
    auto m = build_matrix(distances_csv, os);

    os << "\t Do eigenvalue decomposition..." << endl;
    // do eigenvalue decomposition
    SpectralClustering* c = new SpectralClustering(m, 20);

    // how many clusters you want
    os << "\t Clustering..." << endl;
    auto clusters = c->clusterKmeans(k);

    // output clustered items
    // items are ordered according to distance from cluster centre
    print_to_file(clusters, kmeans_filename);
    return clusters;
}
