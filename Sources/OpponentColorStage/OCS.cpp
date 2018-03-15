//
// Created by Ivan Kirov on 28.02.2018.
//

#include <opencv2/imgproc.hpp>
#include <iostream>
#include "OCS.h"

OCS::OCS(int scale) : OpponentColorStage(scale) {
    getEKernel().copyTo(eGaussianKernel);
    getIKernel().copyTo(iGaussianKernel);
    kernelEI = eGaussianKernel - iGaussianKernel;
    kernelIE = iGaussianKernel - eGaussianKernel;
    cv::threshold(kernelEI, kernelEI, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(kernelIE, kernelIE, 0.0, 255, cv::THRESH_TOZERO);
}

cv::Mat OCS::getEKernel() {
    return cv::getGaussianKernel(kernelSize, sigmaE);
}

cv::Mat OCS::getIKernel() {
    return cv::getGaussianKernel(kernelSize, sigmaI);
}

cv::Mat OCS::getResult(cv::Mat srcL, cv::Mat srcM) { //according to (4)

    //cv::Mat resultKernelEI = eGaussianKernel - iGaussianKernel; //according to (5)
    //cv::threshold(resultKernelEI, resultKernelEI, 0.0, 255, cv::THRESH_TOZERO);

    //cv::Mat resultKernelIE = iGaussianKernel - eGaussianKernel; //according to (5)
    //cv::threshold(resultKernelEI, resultKernelIE, 0.0, 255, cv::THRESH_TOZERO);

    cv::Mat EIfilteredL, IEfilteredM;
    cv::filter2D(srcL, EIfilteredL, srcL.depth(), kernelEI);
    cv::filter2D(srcM, IEfilteredM, srcM.depth(), kernelIE);

    cv::Mat resultImage;
    resultImage = (B*EIfilteredL - C*IEfilteredM) / (A + EIfilteredL + IEfilteredM);

    return resultImage;

}

cv::Mat OCS::getLuminanceOFF() {
    cv::Mat EIfilteredL, IEfilteredM;
    cv::filter2D(luminance, EIfilteredL, luminance.depth(), kernelIE);
    cv::filter2D(luminance, IEfilteredM, luminance.depth(), kernelEI);

    cv::Mat resultImage;
    resultImage = (B*EIfilteredL - C*IEfilteredM) / (A + EIfilteredL + IEfilteredM);

    return resultImage;
}



