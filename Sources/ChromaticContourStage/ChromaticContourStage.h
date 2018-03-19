//
// Created by Ivan Kirov on 01.03.2018.
//

#ifndef DPREEN_SEGMENTATION_CHROMATICCONTOURSTAGE_H
#define DPREEN_SEGMENTATION_CHROMATICCONTOURSTAGE_H


#include <opencv2/core/mat.hpp>
#include "../Model/IModel.h"

#define MAX_SCALE 3
#define MAX_K 6
#define KERNEL_SIZE 3
#define PI 3.1416

class ChromaticContourStage {

public:
    cv::Mat getStageOutput(int scale, int k);
    void init(cv::Mat lon, cv::Mat loff, cv::Mat rg, cv::Mat gr, cv::Mat by, cv::Mat yb); //output of previous stage
    void init(const std::map<OPPONENT, cv::Mat> & in);

private:
    float const A1 = 10.0;
    float const A3 = 1.0;
    float const lambda = 1.2;
    float const gamma = 0.5;
    float const alpha = 1.0;
    float const beta = 0.5;
    float const kLambda = 0.5;


    cv::Mat lon, loff, rg, gr, by, yb;


    std::vector<std::vector<cv::Mat>> evenGaborFilters; //cos real
    std::vector<std::vector<cv::Mat>> oddGaborFilters; //sin img

    //cv::Mat differenceOfGaussFilter;


    float getFrequency(int scale);
    float getSigma(int scale);
    cv::Mat getSimpleCellActivity(cv::Mat src, cv::Mat filter);
    cv::Mat getSimpleCellE(cv::Mat src, int scale, int k);
    cv::Mat getSimpleCellF(cv::Mat src, int scale, int k);
    int getREoutout();
    cv::Mat getHOutput(int scale, int k); //according to 8
    cv::Mat getDifferenceGaussFilter(int k);

public:
    ChromaticContourStage();


};


#endif //DPREEN_SEGMENTATION_CHROMATICCONTOURSTAGE_H
