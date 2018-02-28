//
// Created by Ivan Kirov on 28.02.2018.
//

#include <opencv2/imgproc.hpp>
#include "OCS.h"

OCS::OCS(int scale) : OpponentColorStage(scale) {
    getEKernel().copyTo(eGaussianKernel);
    getIKernel().copyTo(iGaussianKernel);
}

cv::Mat OCS::getEKernel() {
    return cv::getGaussianKernel(kernelSize, sigmaE);
}

cv::Mat OCS::getIKernel() {
    return cv::getGaussianKernel(kernelSize, sigmaI);
}

cv::Mat OCS::getRezult(cv::Mat srcL, cv::Mat srcM) { //according to (4)

    cv::Mat resultKernelEI = eGaussianKernel - iGaussianKernel; //according to (5)
    cv::threshold(resultKernelEI, resultKernelEI, 0.0, 255, cv::THRESH_TOZERO);

    cv::Mat resultKernelIE = iGaussianKernel - eGaussianKernel; //according to (5)
    cv::threshold(resultKernelEI, resultKernelIE, 0.0, 255, cv::THRESH_TOZERO);

    cv::Mat EIfilteredL, IEfilteredM;
    cv::filter2D(srcL, EIfilteredL, srcL.depth(), resultKernelEI);
    cv::filter2D(srcM, IEfilteredM, srcM.depth(), resultKernelIE);

    cv::Mat resultImage;
    resultImage = (B*EIfilteredL - C*IEfilteredM) / (A + EIfilteredL + IEfilteredM);

    return resultImage;

}

