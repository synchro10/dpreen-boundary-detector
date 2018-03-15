//
// Created by Ivan Kirov on 11.03.2018.
//

#ifndef DPREEN_SEGMENTATION_COMPETITIVECOOPERATIVESTAGE_H
#define DPREEN_SEGMENTATION_COMPETITIVECOOPERATIVESTAGE_H


#include <opencv2/core/mat.hpp>

class CompetitiveCooperativeStage {

public:
    CompetitiveCooperativeStage();
    cv::Mat getStageOutput(int scale);

private:
    cv::Mat getCFOutput(); //according to (9)
    cv::Mat getCSOutput(); //according to (10)

    float const A4 = 10.0;
    float const Kh = 1.0;
    float const Kf = 5.0;
    float const Cc = 0.3;
    float const Ci = 0.1;
    int const gaussianKernelSize = 5;
    int const bipKernelSize = 5;

    cv::Mat getGaussianKernel(float r, float tau, float sigma); //to get rotated(r grades) gaussian with custom circular symmetry(tau)

    cv::Mat getBipKernel(float k, float kl, float dc); //to get BIP kernel


};


#endif //DPREEN_SEGMENTATION_COMPETITIVECOOPERATIVESTAGE_H
