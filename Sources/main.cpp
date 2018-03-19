#include <iostream>
#include <opencv/cv.hpp>
#include "OpponentColorStage/OpponentColorStage.h"
#include "ChromaticContourStage/ChromaticContourStage.h"
#include "OpponentColorStage/OpponentColorStage.h"
#include "Utility/Util.h"

#define SCALE 1
#define EPS 0.0001


using namespace cv;
using namespace std;

int main() {

    Mat img = imread( "../pictures/1.jpg", CV_LOAD_IMAGE_COLOR);
    if(! img.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }


    OpponentColorStage ocs(SCALE);
    ocs.init(img);
    cv::Mat DBY = ocs.getDBY();
    cv::Mat DYB = ocs.getDYB();
    cv::Mat DGR = ocs.getDGR();
    cv::Mat DRG = ocs.getDRG();
    cv::Mat luminanceON = ocs.getLuminanceON();
    cv::Mat luminanceOFF = ocs.getLuminanceOFF();

    //imshow( "Display DBY", DBY);                   // Show our image inside it.
/*
    imshow( "Display DYB", DYB);                   // Show our image inside it.
    imshow( "Display DGR", DGR);                   // Show our image inside it.
    imshow( "Display DRG", DRG);                   // Show our image inside it.
    imshow( "Display loff", luminanceOFF);                   // Show our image inside it.
    imshow( "Display lon", luminanceON);                   // Show our image inside it.
*/

    ChromaticContourStage ccs;
    ccs.init(luminanceON, luminanceOFF, DRG, DGR, DBY, DYB);
    cv::Mat ccsOut = ccs.getStageOutput(SCALE, 3);
    Util::printMat(ccsOut);
    //cv::Mat eOut = ccs.getSimpleCellE(DBY, SCALE, 1);
    //cv::Mat fOut = ccs.getSimpleCellF(DBY, SCALE, 1);

    //Util::printMat(fOut);
    //imshow( "Display eOut", eOut);                   // Show our image inside it.

    waitKey(0);


}