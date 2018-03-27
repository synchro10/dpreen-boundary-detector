#include <iostream>
#include <opencv/cv.hpp>
#include "Utility/Util.h"
#include "Model/Model.h"

using namespace cv;
using namespace std;

int main() {

    Mat img = imread( "../pictures/113044.jpg", CV_LOAD_IMAGE_COLOR);
    if(! img.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    Model model;
    model.init(img);

    Mat result = model.GetResult();
    Util::printMat(result, "out");

    waitKey(0);
    return 0;
}