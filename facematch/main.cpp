#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv ) {
    if ( argc != 2 ) {
        printf("usage: facematch <path/to/images>\n");
        return -1;
    }

    // Read images and build curated dataset:
    //  * read all images in the input folder and subfolders recursively

    //  * work on each image to get faces
    //  * crop and rescale

    //  * apply unsupervised classification: kmeans
    //  * store each cluster into a different folder and name the photos with numbers
    //  proportional to the distance to the centroid.
    waitKey(0);

    return 0;
}
