//
// Created by kon on 19.03.18.
//

#ifndef DPREEN_SEGMENTATION_UTIL_H
#define DPREEN_SEGMENTATION_UTIL_H

#include <iostream>
#include <opencv/cv.hpp>

class Util {
public:
    static void printMat(const cv::Mat &mat, const std::string & name = "img");
    static void logMat(const cv::Mat & img, const std::string & name);
    static bool matLessScalar(const cv::Mat & mat, const double scalar);
    static cv::Mat perElementMax(std::initializer_list<cv::Mat> list);
    static cv::Mat perElementMax(const std::vector<cv::Mat> & list);
    static void normalize(cv::Mat &src, double low, double high);
    static void
    saveImg(const cv::Mat &src, const std::string &name, const int iteration = 0, const std::string &stage = "result");
};


#endif //DPREEN_SEGMENTATION_UTIL_H
