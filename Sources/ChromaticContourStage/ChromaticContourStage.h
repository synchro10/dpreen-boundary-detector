//
// Created by Ivan Kirov on 01.03.2018.
//

#ifndef DPREEN_SEGMENTATION_CHROMATICCONTOURSTAGE_H
#define DPREEN_SEGMENTATION_CHROMATICCONTOURSTAGE_H


#include <opencv2/core/mat.hpp>
#define MAX_SCALE 3
#define MAX_K 6
#define KERNEL_SIZE 3
#define PI 3.1416

class ChromaticContourStage {

public:
    cv::Mat getSimpleCellE(cv::Mat src, int scale, int k);
    cv::Mat getSimpleCellF(cv::Mat src, int scale, int k);

private:
    float const A1 = 10.0;
    float const lambda = 1.2;

    std::vector<std::vector<cv::Mat>> evenGaborFilters; //cos real
    std::vector<std::vector<cv::Mat>> oddGaborFilters; //sin img


    float getFrequencie(int scale);
    float getSigma(int scale);
    cv::Mat getResult(cv::Mat src, cv::Mat filter);

public:
    ChromaticContourStage();


};


#endif //DPREEN_SEGMENTATION_CHROMATICCONTOURSTAGE_H
