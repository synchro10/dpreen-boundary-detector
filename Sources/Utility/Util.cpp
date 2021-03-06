//
// Created by kon on 19.03.18.
//

#include "Util.h"

void Util::printMat(const cv::Mat &mat, const std::string & name) {
    cv::Mat tmp;
    cv::normalize(mat, tmp, 255, 0);
    imshow(name, tmp);                   // Show our image inside it.
    cv::waitKey(0);
}

void Util::logMat(const cv::Mat &img, const std::string &name) {
    //cv::Mat ef;
    //img.convertTo(ef, CV_8UC3);
    cv::imwrite("../pictures/log/" + name + ".jpg", img);
    //todo
}

bool Util::matLessScalar(const cv::Mat & mat, const double scalar) {
    for(int i = 1; i < mat.rows - 1; i++){
        for(int j = 1; j < mat.cols - 1; j++){
            float val = mat.at<float>(i, j);
            if (val >= scalar){
                return false;
            }
        }
    }
    return true;
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

void Util::normalize(cv::Mat &src, double low, double high) {
    cv::normalize(src, src, low, high);
}

void Util::saveImg(const cv::Mat &src, const std::string &name, const int iteration, const std::string &stage) {
    cv::Mat tmp = src;
    cv::normalize(src, tmp, 255.0, 0.0);
    tmp.convertTo(tmp, CV_8UC4, 255.0, 0.0);
    std::string fileName;
    if (stage != "result"){
        fileName = "../pictures/" + stage + "/" + name + "/" + std::to_string(iteration) + "_iter" + ".jpg";
    } else {
        fileName = "../pictures/" + name + ".jpg";
    }
    cv::imwrite(fileName, tmp);
}
