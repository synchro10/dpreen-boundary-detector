//
// Created by Ivan Kirov on 01.03.2018.
//

#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv/cv.hpp>
#include "ChromaticContourStage.h"

ChromaticContourStage::ChromaticContourStage() {
    for(int s = 0; s < MAX_SCALE; s++){
        std::vector<cv::Mat> evenFilters;
        std::vector<cv::Mat> oddFilters;

        for(int k = 0; k < MAX_K; k++){

            evenFilters.push_back(cv::getGaborKernel(cv::Size(KERNEL_SIZE, KERNEL_SIZE), getSigma(s), (k+1) * 30, lambda, getFrequencie(k), 0., CV_32FC1));
            oddFilters.push_back(cv::getGaborKernel(cv::Size(KERNEL_SIZE, KERNEL_SIZE), getSigma(s), (k+1) * 30, lambda, getFrequencie(k), CV_PI/2., CV_32FC1));
            //evenFilter = cv::getGaborKernel(cv::Size(KERNEL_SIZE, KERNEL_SIZE), getSigma(s), (k+1) * 30, lambda, getFrequencie(k), 0., CV_32FC1); //real
            //oddFilter = cv::getGaborKernel(cv::Size(KERNEL_SIZE, KERNEL_SIZE), getSigma(s), (k+1) * 30, lambda, getFrequencie(k), PI/2., CV_32FC1); //img

        }

        evenGaborFilters.push_back(evenFilters);
        oddGaborFilters.push_back(oddFilters);

    }
}


float ChromaticContourStage::getFrequencie(int scale) {

    switch(scale){
        case 0:
            return 0.15;
        case 1:
            return 0.07;
        case 2:
            return 0.03;
    }
}

float ChromaticContourStage::getSigma(int scale) {
    return std::pow(2, scale);
}

cv::Mat ChromaticContourStage::getSimpleCellE(cv::Mat src, int scale, int k) {
    return getResult(src, evenGaborFilters[scale][k]);
}

cv::Mat ChromaticContourStage::getSimpleCellF(cv::Mat src, int scale, int k) {
    return getResult(src, oddGaborFilters[scale][k]);
}

cv::Mat ChromaticContourStage::getResult(cv::Mat src, cv::Mat filter) {

    cv::Mat absFilter;
    cv::threshold(filter, absFilter, 0.0, 255, cv::THRESH_TOZERO);

    cv::Mat filtered;
    cv::filter2D(src, filtered, src.depth(), filter);

    cv::Mat absFiltered;
    cv::filter2D(src, absFiltered, src.depth(), absFilter);

    cv::Mat result;
    result = filtered / (A1 + absFiltered);
    return result;

}
