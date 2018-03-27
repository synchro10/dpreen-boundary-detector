#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "OpponentColorStage.h"


OpponentColorStage::OpponentColorStage(int scale) : scale(scale) {

    sigmaE = std::pow(2, scale);
    sigmaI = 1.5 * sigmaE;

    getEKernel().copyTo(eGaussianKernel);
    getIKernel().copyTo(iGaussianKernel);
    kernelEI = eGaussianKernel - iGaussianKernel;
    kernelIE = iGaussianKernel - eGaussianKernel;
    cv::threshold(kernelEI, kernelEI, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(kernelIE, kernelIE, 0.0, 255, cv::THRESH_TOZERO);
}

void OpponentColorStage::init(cv::Mat src) {

    std::vector<cv::Mat> splited_img = std::vector<cv::Mat>();
    cv::split(src, splited_img);
    splited_img[2].copyTo(red);
    splited_img[1].copyTo(green);
    splited_img[0].copyTo(blue);
    yellow = splited_img[1] * 0.5 + splited_img[2] * 0.5;
    luminance = splited_img[2] * 0.299 + splited_img[1] * 0.587 + splited_img[0] * 0.114;
    red.convertTo(red, CV_32FC1);
    green.convertTo(green, CV_32FC1);
    blue.convertTo(blue, CV_32FC1);
    yellow.convertTo(yellow, CV_32FC1);
    luminance.convertTo(luminance, CV_32FC1);

}

cv::Mat OpponentColorStage::getResult(cv::Mat srcL, cv::Mat srcM) {
    //cv::Mat resultKernelEI = eGaussianKernel - iGaussianKernel; //according to (5)
    //cv::threshold(resultKernelEI, resultKernelEI, 0.0, 255, cv::THRESH_TOZERO);

    //cv::Mat resultKernelIE = iGaussianKernel - eGaussianKernel; //according to (5)
    //cv::threshold(resultKernelEI, resultKernelIE, 0.0, 255, cv::THRESH_TOZERO);

    cv::Mat EIfilteredL, IEfilteredM;
    cv::filter2D(srcL, EIfilteredL, srcL.depth(), kernelEI);
    cv::filter2D(srcM, IEfilteredM, srcM.depth(), kernelIE);

    cv::Mat resultImage;
    resultImage = (B*EIfilteredL - C*IEfilteredM) / (A + EIfilteredL + IEfilteredM);

    return std::move(resultImage);
}

cv::Mat OpponentColorStage::getLuminanceOFF() {
    cv::Mat EIfilteredL, IEfilteredM;
    cv::filter2D(luminance, EIfilteredL, luminance.depth(), kernelIE);
    cv::filter2D(luminance, IEfilteredM, luminance.depth(), kernelEI);

    cv::Mat resultImage;
    resultImage = (B*EIfilteredL - C*IEfilteredM) / (A + EIfilteredL + IEfilteredM);

    return std::move(resultImage);
}


cv::Mat OpponentColorStage::getDGR() {
    return getResult(green, red);
}

cv::Mat OpponentColorStage::getDRG() {
    return getResult(red, green);
}

cv::Mat OpponentColorStage::getDBY() {
    return getResult(blue, yellow);
}

cv::Mat OpponentColorStage::getDYB() {
    return getResult(yellow, blue);
}

cv::Mat OpponentColorStage::getLuminanceON() {
    return getResult(luminance, luminance);
}

cv::Mat OpponentColorStage::getEKernel() {
    return cv::getGaussianKernel(kernelSize, sigmaE);
}

cv::Mat OpponentColorStage::getIKernel() {
    return cv::getGaussianKernel(kernelSize, sigmaI);
}

std::map<OPPONENT, cv::Mat> OpponentColorStage::GetOutput() {
    std::map<OPPONENT, cv::Mat> v;
    v[RG] = getDRG();
    v[GR] = getDGR();
    v[BY] = getDBY();
    v[YB] = getDYB();
    v[ON] = getLuminanceON();
    v[OFF] = getLuminanceOFF();
    return std::move(v);
}

const cv::Mat & OpponentColorStage::getKernelEI() {
    return kernelEI;
}

const cv::Mat & OpponentColorStage::getKernelIE() {
    return kernelIE;
}
