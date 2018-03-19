//
// Created by kon on 19.03.18.
//

#ifndef DPREEN_SEGMENTATION_UTIL_H
#define DPREEN_SEGMENTATION_UTIL_H

#include <iostream>
#include <opencv/cv.hpp>

class Util {
public:
    static void printMat(const cv::Mat &mat);
    static void logMat(const cv::Mat & img, const std::string & name);
};


#endif //DPREEN_SEGMENTATION_UTIL_H
