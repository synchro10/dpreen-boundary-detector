#include <iostream>
#include <opencv/cv.hpp>
#include "OpponentColorStage/interface/OpponentColorStage.h"
#include "OpponentColorStage/OCS.h"

#define SCALE 1

using namespace cv;
using namespace std;

void printMat(const cv::Mat &mat){
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

    //printMat(img);

    OCS ocs(SCALE);
    ocs.init(img);
    cv::Mat DBY = ocs.getDBY();
    cv::Mat DYB = ocs.getDYB();
    cv::Mat DGR = ocs.getDGR();
    cv::Mat DRG = ocs.getDRG();
    cv::Mat luminanceON = ocs.getLuminanceON();
    cv::Mat luminanceOFF = ocs.getLuminanceOFF();


    imshow( "Display DBY", DBY);                   // Show our image inside it.
    imshow( "Display DYB", DYB);                   // Show our image inside it.
    imshow( "Display DGR", DGR);                   // Show our image inside it.
    imshow( "Display DRG", DRG);                   // Show our image inside it.
    imshow( "Display loff", luminanceOFF);                   // Show our image inside it.
    imshow( "Display lon", luminanceON);                   // Show our image inside it.


    waitKey(0);


}