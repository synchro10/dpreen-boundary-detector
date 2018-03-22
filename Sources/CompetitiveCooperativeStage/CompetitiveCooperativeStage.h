//
// Created by Ivan Kirov on 11.03.2018.
//

#ifndef DPREEN_SEGMENTATION_COMPETITIVECOOPERATIVESTAGE_H
#define DPREEN_SEGMENTATION_COMPETITIVECOOPERATIVESTAGE_H


#include <opencv2/core/mat.hpp>
#include "../Model/IModel.h"
#include "../GlobalConstants.h"
#include <vector>

class CompetitiveCooperativeStage {

public:
    CompetitiveCooperativeStage();
    std::vector<cv::Mat> getStageOutput(int scale);
    void init(std::vector<cv::Mat> H); // output from V1

private:
    void calcCFOutput(const int scale); //according to (9)
    void calcCSOutput(const int scale); //according to (10)

    //for k = 0..MAX_K-1
    std::vector<cv::Mat> U = std::vector<cv::Mat>(MAX_K);
    std::vector<cv::Mat> Uold = std::vector<cv::Mat>(MAX_K);
    std::vector<cv::Mat> F = std::vector<cv::Mat>(MAX_K);
    std::vector<cv::Mat> H = std::vector<cv::Mat>(MAX_K);

    float const A4 = 10.0f;
    float const Kh = 1.0f;
    float const Kf = 5.0f;
    float const Cc = 0.3f;
    float const Ci = 0.1f;
    int const gaussianKernelSize = 5;
    int const bipKernelSize = 5;
    const float sigmaI[3] = {1.0f, 2.0f, 4.0f};
    const float sigmaU[3] = {0.4f, 0.8f, 1.0f};
    const float tauJ = 7.0f;
    const float tauG = 1.0f;

    const float A5 = 0.001f;
    const float tau = 1.2f;
    const float alpha = 0.1f;
    const float Kl[3] = {0.8f, 0.4f, 0.2f};
    const float Dc[3] = {4.f, 10.f, 14.f};
    const float eps = 0.0001f;

    cv::Mat getGaussianKernel(float r, float tau, float sigma); //to get rotated(r grades) gaussian with custom circular symmetry(tau)

    cv::Mat getBipKernel(float k, float kl, float dc); //to get BIP kernel

};


#endif //DPREEN_SEGMENTATION_COMPETITIVECOOPERATIVESTAGE_H
