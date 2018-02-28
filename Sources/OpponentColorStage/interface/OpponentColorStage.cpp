#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "OpponentColorStage.h"


OpponentColorStage::OpponentColorStage(int scale) : scale(scale) {

    sigmaE = std::pow(2, scale);
    sigmaI = 1.5 * sigmaE;

}



void OpponentColorStage::init(cv::Mat src) {

    std::vector<cv::Mat> splited_img = std::vector<cv::Mat>();
    cv::split(src, splited_img);
    splited_img[2].copyTo(red);
    splited_img[1].copyTo(green);
    splited_img[0].copyTo(blue);
    yellow = splited_img[1] * 0.5 + splited_img[2] * 0.5;
    red.convertTo(red, CV_32FC1);
    green.convertTo(green, CV_32FC1);
    blue.convertTo(blue, CV_32FC1);
    yellow.convertTo(yellow, CV_32FC1);

}

cv::Mat OpponentColorStage::getDGR() {
    return getRezult(green, red);
}

cv::Mat OpponentColorStage::getDRG() {
    return getRezult(red, green);
}

cv::Mat OpponentColorStage::getDBY() {
    return getRezult(blue, yellow);
}

cv::Mat OpponentColorStage::getDYB() {
    return getRezult(yellow, blue);
}
