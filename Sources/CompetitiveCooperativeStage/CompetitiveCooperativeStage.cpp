//
// Created by Ivan Kirov on 11.03.2018.
//

#include <opencv2/imgproc.hpp>
#include "CompetitiveCooperativeStage.h"

CompetitiveCooperativeStage::CompetitiveCooperativeStage() {

}

cv::Mat CompetitiveCooperativeStage::getStageOutput(int scale) {
    return cv::Mat();
}

/*
 * Для генерации ядер P и N
 *
 * Названия параметров соответсвуют статье
 *
 * */
cv::Mat CompetitiveCooperativeStage::getBipKernel(float k, float kl, float dc) {

    cv::Mat kernel = cv::Mat::zeros(bipKernelSize, bipKernelSize, CV_32FC1);

    const auto y_mid = (bipKernelSize-1) / 2.0;
    const auto x_mid = (bipKernelSize-1) / 2.0;

    float tau = 1.2;
    double angle = CV_PI / 6 * k;

/*    const auto x_spread = 1. / (pow(sigma, 2)*2);
    const auto y_spread = 1. / (pow(sigma, 2)*2);*/

    //const auto denominator = 8 * std::atan(1) * sigma * sigma;

    std::vector<std::vector<double> > bip(
            bipKernelSize,
            std::vector<double>(bipKernelSize));


    for (auto i = 0;  i < bipKernelSize;  ++i)  {
        auto x = i - x_mid;
        for(auto j = 0; j < bipKernelSize; ++j){
            auto y = j - y_mid;

            auto x_= (x - dc * std::cos(angle))*std::cos(angle) + (y - dc * std::sin(angle))*std::sin(angle);
            auto y_= (y - dc * std::cos(angle))*std::sin(angle) + (y - dc * std::sin(angle))*std::cos(angle);

            bip[i][j] = std::exp(-kl * std::sqrt( std::pow(x_/tau, 2) + std::pow(y_, 2) ));
        }

    }

    float denominator = cv::norm(kernel, cv::NORM_L1);

    for (auto i = 0;  i < bipKernelSize;  ++i) {
        for (auto j = 0; j < bipKernelSize; ++j) {
            kernel.at<float>(j, i) = static_cast<float>(bip[i][j] / denominator);
        }
    }

    return kernel;
}

/*
 * Для генерации гауссова ядра с углом поворота r, эллиптичностью tau, sigma - дисперсия
 *
 * Для генерации ядер J и G
 *
 * */
cv::Mat CompetitiveCooperativeStage::getGaussianKernel(float r, float tau, float sigma) {
    cv::Mat gaussianKernel = cv::Mat::zeros(gaussianKernelSize, gaussianKernelSize, CV_32FC1);

    const auto y_mid = (gaussianKernelSize-1) / 2.0;
    const auto x_mid = (gaussianKernelSize-1) / 2.0;

    const auto x_spread = 1. / (pow(sigma, 2)*2);
    const auto y_spread = 1. / (pow(sigma, 2)*2);

    const auto denominator = 8 * std::atan(1) * sigma * sigma;

    std::vector<std::vector<double> > gauss(
            gaussianKernelSize,
            std::vector<double>(gaussianKernelSize));


    for (auto i = 0;  i < gaussianKernelSize;  ++i)  {
        auto x = i - x_mid;
        for(auto j = 0; j < gaussianKernelSize; ++j){
            auto y = j - y_mid;
            gauss[i][j] = std::exp(-std::pow(x, 2) * x_spread
                                   -std::pow(y/tau,2) * y_spread);
        }

    }


    for (auto i = 0;  i < gaussianKernelSize;  ++i) {
        for (auto j = 0; j < gaussianKernelSize; ++j) {
            gaussianKernel.at<float>(j, i) = static_cast<float>(gauss[i][j] / denominator);
        }
    }

    cv::Point2f src_center(gaussianKernel.cols/2.0F, gaussianKernel.rows/2.0F);
    cv::Mat rot_mat = cv::getRotationMatrix2D(src_center, r, 1.0);
    warpAffine(gaussianKernel, gaussianKernel, rot_mat, gaussianKernel.size());

    return gaussianKernel;
}


