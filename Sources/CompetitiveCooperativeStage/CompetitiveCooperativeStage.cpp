//
// Created by Ivan Kirov on 11.03.2018.
//

#include <opencv2/imgproc.hpp>
#include "CompetitiveCooperativeStage.h"

cv::Mat CompetitiveCooperativeStage::getGaussianKernel(float r, float tau, float sigma) {
    cv::Mat gaussianKernel = cv::Mat::zeros(kernelSize, kernelSize, CV_32FC1);

    const auto y_mid = (kernelSize-1) / 2.0;
    const auto x_mid = (kernelSize-1) / 2.0;

    const auto x_spread = 1. / (pow(sigma, 2)*2);
    const auto y_spread = 1. / (pow(sigma, 2)*2);

    const auto denominator = 8 * std::atan(1) * sigma * sigma;

    std::vector<std::vector<double> > gauss(
            kernelSize,
            std::vector<double>(kernelSize));


    for (auto i = 0;  i < kernelSize;  ++i)  {
        auto x = i - x_mid;
        for(auto j = 0; j < kernelSize; ++j){
            auto y = j - y_mid;
            gauss[i][j] = std::exp(-std::pow(x, 2) * x_spread
                                   -std::pow(y/tau,2) * y_spread);
        }

    }


    for (auto i = 0;  i < kernelSize;  ++i) {
        for (auto j = 0; j < kernelSize; ++j) {
            gaussianKernel.at<float>(j, i) = static_cast<float>(gauss[i][j] / denominator);
        }
    }

    cv::Point2f src_center(gaussianKernel.cols/2.0F, gaussianKernel.rows/2.0F);
    cv::Mat rot_mat = cv::getRotationMatrix2D(src_center, r, 1.0);
    warpAffine(gaussianKernel, gaussianKernel, rot_mat, gaussianKernel.size());

    return gaussianKernel;
}
