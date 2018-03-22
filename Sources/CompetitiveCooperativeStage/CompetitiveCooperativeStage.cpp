//
// Created by Ivan Kirov on 11.03.2018.
//

#include <opencv2/imgproc.hpp>
#include "CompetitiveCooperativeStage.h"
#include <vector>
#include "../Utility/Util.h"

CompetitiveCooperativeStage::CompetitiveCooperativeStage() {

}

std::vector<cv::Mat> CompetitiveCooperativeStage::getStageOutput(int scale) {
    calcCFOutput(scale);
    calcCSOutput(scale);
    int iterations = 1;
    bool isStop;
    do {
        calcCFOutput(scale);
        calcCSOutput(scale);
        isStop = true;
        for(int i = 0; i < MAX_K; i++){
            isStop &= Util::matLessScalar(cv::abs(U[i] - Uold[i]), eps);
        }
        ++iterations;
    } while(!isStop);
    std::cout << "iterations in V2 stage: " << iterations << std::endl;
    //std::cout << cv::abs(U[0] - Uold[0]) << std::endl;
    return U;
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

    for (auto i = 0;  i < bipKernelSize;  ++i) {
        for (auto j = 0; j < bipKernelSize; ++j) {
            kernel.at<float>(j, i) = static_cast<float>(bip[i][j]);
        }
    }

    float denominator = cv::norm(kernel, cv::NORM_L1);
    kernel = kernel / denominator;

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

void CompetitiveCooperativeStage::calcCFOutput(const int scale) {
    Uold.swap(U);
    std::vector<cv::Mat> JU;
    for(int i = 0; i < MAX_K; i++){
        cv::Mat tmp;
        cv::Mat jFilter = getGaussianKernel(i, tauJ, sigmaI[scale]);
        cv::filter2D(Uold[i], tmp, Uold[i].depth(), jFilter);
        JU.push_back(std::move(tmp));
    }

    for (int i = 0; i < MAX_K; ++i){
        cv::Mat u = Kh * H[i] + Kf * F[i];

        cv::Mat GU;
        cv::Mat gFilter = getGaussianKernel(0.0f, tauG, sigmaU[scale]);
        cv::filter2D(Uold[i], GU, Uold[i].depth(), gFilter);

        cv::Mat summ = cv::Mat(JU[0].rows, JU[0].cols, JU[0].type(), 0.f);
        for(int j = 0; j < MAX_K; ++j) {
            if (j == i)
                continue;
            summ += JU[j];
        }

        U[i] = (u - Ci * summ - Cc * GU) / (A4 + u + summ + GU);
        cv::threshold(U[i], U[i], 0.0, 255, cv::THRESH_TOZERO);
    }
}

void CompetitiveCooperativeStage::calcCSOutput(const int scale) {
    for(int i = 0; i < MAX_K; ++i){
        cv::Mat PU, NU, zPU, zNU;
        cv::Mat pFilter = getBipKernel(i, Kl[scale], Dc[scale]);
        cv::Mat nFilter = getBipKernel(i, Kl[scale], (-1.f)*Dc[scale]);
        cv::filter2D(U[i], PU, U[i].depth(), pFilter);
        cv::filter2D(U[i], NU, U[i].depth(), nFilter);

        cv::max(PU - alpha, 0.0, zPU);
        cv::max(NU - alpha, 0.0, zNU);

        std::cout << U[i] << std::endl;
        std::cout << pFilter << std::endl;
        std::cout << nFilter << std::endl;
        std::cout << PU << std::endl;
        std::cout << NU << std::endl;
        F[i] = zPU.mul(zNU) / (A5 + PU.mul(NU));
    }
    std::cout << F[0] << std::endl;
}

void CompetitiveCooperativeStage::init(std::vector<cv::Mat> H) {
    for(int i = 0; i < MAX_K; i++){
        U[i] = cv::Mat(H[0].rows, H[0].cols, H[0].type(), 0.f);
        U[i].depth();
        Uold[i] = cv::Mat(H[0].rows, H[0].cols, H[0].type(), 0.f);
        F[i] = cv::Mat(H[0].rows, H[0].cols, H[0].type(), 0.f);
    }
    this->H = std::move(H);
}


