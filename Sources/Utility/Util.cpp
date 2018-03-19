//
// Created by kon on 19.03.18.
//

#include "Util.h"

void Util::printMat(const cv::Mat &mat) {
    normalize(mat, mat, 255, 0);
    imshow( "Display mat", mat);                   // Show our image inside it.
    //waitKey(0);
}

void Util::logMat(const cv::Mat &img, const std::string &name) {
    //cv::Mat ef;
    //img.convertTo(ef, CV_8UC3);
    cv::imwrite("../pictures/log/" + name + ".jpg", img);
    //todo
}
