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

void Util::normalize(cv::Mat &src, double low, double high) {
    cv::normalize(src, src, low, high);
}

void Util::saveImg(const cv::Mat &src, const std::string &name) {
    cv::Mat tmp;
    cv::normalize(src, tmp, 255.0, 0.0);
  //  tmp.convertTo(tmp, CV_8U);
    cv::Mat tmp2 = tmp;
    cv::Mat tmp3 = tmp;
    std::vector<cv::Mat> splitedImg;
    splitedImg.emplace_back(tmp);
    splitedImg.emplace_back(tmp2);

    splitedImg.emplace_back(tmp3);

    cv::merge(splitedImg, tmp);
    cv::imwrite("../pictures/" + name + ".jpg", tmp);
}
