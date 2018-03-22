//
// Created by Ivan Kirov on 01.03.2018.
//

#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv/cv.hpp>
#include <map>
#include "ChromaticContourStage.h"

ChromaticContourStage::ChromaticContourStage() {
    for(int s = 0; s < MAX_SCALE; s++){
        std::vector<cv::Mat> evenFilters;
        std::vector<cv::Mat> oddFilters;

        for(int k = 0; k < MAX_K; k++){

            evenFilters.push_back(cv::getGaborKernel(cv::Size(KERNEL_SIZE, KERNEL_SIZE), getSigma(s), (k+1) * 30, lambda, gamma, 0., CV_32FC1));
            oddFilters.push_back(cv::getGaborKernel(cv::Size(KERNEL_SIZE, KERNEL_SIZE), getSigma(s), (k+1) * 30, lambda, gamma, CV_PI/2., CV_32FC1));

        }

        evenGaborFilters.push_back(evenFilters);
        oddGaborFilters.push_back(oddFilters);

    }

}


float ChromaticContourStage::getFrequency(int scale) {

    switch(scale){
        case 0:
            return 0.15f;
        case 1:
            return 0.07f;
        case 2:
            return 0.03f;
        default:
            return 0.f;
    }
}

double ChromaticContourStage::getSigma(int scale) {
    return std::pow(2, scale);
}

cv::Mat ChromaticContourStage::getSimpleCellE(cv::Mat src, int scale, int k) {
    return getSimpleCellActivity(std::move(src), evenGaborFilters[scale][k]);
}

cv::Mat ChromaticContourStage::getSimpleCellF(cv::Mat src, int scale, int k) {
    return getSimpleCellActivity(std::move(src), oddGaborFilters[scale][k]);
}

cv::Mat ChromaticContourStage::getSimpleCellActivity(cv::Mat src, cv::Mat filter) {

    cv::Mat absFilter;
    cv::Mat temp1, temp2;
    cv::threshold(filter, temp1, 0.0, 255, cv::THRESH_TOZERO);
    temp2 = filter * -1;
    cv::threshold(temp2, temp2, 0.0, 255, cv::THRESH_TOZERO);
    absFilter = temp1 + temp2;


    cv::Mat filtered;
    cv::filter2D(src, filtered, src.depth(), filter);

    cv::Mat absFiltered;
    cv::filter2D(src, absFiltered, src.depth(), absFilter);

    cv::Mat result;
    result = filtered / (A1 + absFiltered);
    return result;

}

cv::Mat ChromaticContourStage::getHOutput(int scale, int k) {

    cv::Mat eLON, eLOFF, eRG, eGR, eBY, eYB;
    cv::Mat fLON, fLOFF, fRG, fGR, fBY, fYB;

    eLOFF = getSimpleCellE(loff, scale, k);
    eLON = getSimpleCellE(lon, scale, k);
    eRG = getSimpleCellE(rg, scale, k);
    eGR = getSimpleCellE(gr, scale, k);
    eBY = getSimpleCellE(by, scale, k);
    eYB = getSimpleCellE(yb, scale, k);

    fLOFF = getSimpleCellF(loff, scale, k);
    fLON = getSimpleCellF(lon, scale, k);
    fRG = getSimpleCellF(rg, scale, k);
    fGR = getSimpleCellF(gr, scale, k);
    fBY = getSimpleCellF(by, scale, k);
    fYB = getSimpleCellF(yb, scale, k);

    cv::threshold(eLOFF, eLOFF, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(eLON, eLON, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(eRG, eRG, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(eGR, eGR, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(eBY, eBY, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(eYB, eYB, 0.0, 255, cv::THRESH_TOZERO);

    cv::threshold(fLOFF, fLOFF, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(fLON, fLON, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(fRG, fRG, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(fGR, fGR, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(fBY, fBY, 0.0, 255, cv::THRESH_TOZERO);
    cv::threshold(fYB, fYB, 0.0, 255, cv::THRESH_TOZERO);

    cv::Mat result;
    result = alpha * (eLOFF + fLOFF + eLON + fLON + eRG + fRG + eGR + fGR + eBY + fBY + eYB + fYB) + beta * getREoutout();
    return result;
}

void ChromaticContourStage::init(cv::Mat lon, cv::Mat loff, cv::Mat rg, cv::Mat gr, cv::Mat by, cv::Mat yb) {
    this->lon = std::move(lon);
    this->loff = std::move(loff);
    this->rg = std::move(rg);
    this->gr = std::move(gr);
    this->by = std::move(by);
    this->yb = std::move(yb);
}

void ChromaticContourStage::init(const std::map<OPPONENT, cv::Mat> & in) {
    this->lon = in.at(ON);
    this->loff = in.at(OFF);
    this->rg = in.at(RG);
    this->gr = in.at(GR);
    this->by = in.at(BY);
    this->yb = in.at(YB);
}

int ChromaticContourStage::getREoutout() {
    //TODO
    return 1;
}

std::vector<cv::Mat> ChromaticContourStage::getStageOutput(int scale) {
    std::vector<cv::Mat> out;
    for (int i = 0; i < MAX_K; i++)
    {
        cv::Mat hOutput = getHOutput(scale, i);
    /*    cv::imshow( "Display h", hOutput);                   // Show our image inside it.
        cv::waitKey(0);*/
        cv::Mat filter = getDifferenceGaussFilter(i);
        cv::Mat filtered;
        cv::filter2D(hOutput,filtered, hOutput.depth(), filter);
        cv::Mat result;

        result = (hOutput - kLambda * filtered) / (A3 + hOutput - filtered);
        //cv::threshold(result, result, 0.0, 255, cv::THRESH_TOZERO);
        out.push_back(std::move(result));
    }
    return std::move(out);

}

cv::Mat ChromaticContourStage::getDifferenceGaussFilter(int k) {
    double sigmaE = std::pow(2, k);
    double sigmaI = 2 * sigmaE;

    cv::Mat kernelE, kernelI;
    kernelE = cv::getGaussianKernel(3, sigmaE);
    kernelI = cv::getGaussianKernel(3, sigmaI);
    cv::Mat kernelIE = kernelI - kernelE;
    cv::threshold(kernelIE, kernelIE, 0.0, 255, cv::THRESH_TOZERO);
    return kernelIE;
}
