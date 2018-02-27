#include <iostream>
#include <opencv/cv.hpp>

using namespace cv;
using namespace std;

void printMat(cv::Mat mat){
    //normalize(mat, mat, 255, 0);
    imshow( "Display mat", mat);                   // Show our image inside it.
    waitKey(0);
}

int main() {

    Mat img = imread( "../pictures/1.jpg", CV_LOAD_IMAGE_COLOR);

    if(! img.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    printMat(img);

}