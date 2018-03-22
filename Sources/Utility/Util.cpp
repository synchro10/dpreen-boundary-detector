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

bool Util::matLessScalar(const cv::Mat & mat, const double scalar) {
    cv::Mat tmp = mat < scalar;
    bool is = true;
    for(int i = 0; i < mat.rows; i++){
        for(int j = 0; j < mat.cols; j++){
            is &= (mat.at<float>(i, j) < scalar);
        }
    }
    return is;
}

cv::Mat Util::perElementMax(std::initializer_list<cv::Mat> list) {
    cv::Mat max;
    list.begin()->copyTo(max);
    for(const auto &elem : list){
        cv::max(max, elem, max);
    }
    return std::move(max);
}

cv::Mat Util::perElementMax(const std::vector<cv::Mat> & list) {
    cv::Mat max;
    list.begin()->copyTo(max);
    for(const auto &elem : list){
        cv::max(max, elem, max);
    }
    return std::move(max);
}
