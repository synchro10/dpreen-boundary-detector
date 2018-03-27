//
// Created by kon on 25.03.18.
//

#ifndef DPREEN_SEGMENTATION_REGIONENHANCEMENTSTAGE_H
#define DPREEN_SEGMENTATION_REGIONENHANCEMENTSTAGE_H

#include <opencv2/core/mat.hpp>
#include "../Model/IModel.h"
#include "../GlobalConstants.h"
#include <vector>
#include <memory>

class RegionEnhancementStage {
public:
    RegionEnhancementStage();
    void init(std::vector<cv::Mat> U, const int scale, IModel *model); //from V2
    std::map<OPPONENT, cv::Mat> getStageOut();
private:
    IModel* model;

    std::vector<cv::Mat> U;
    std::map<OPPONENT , cv::Mat> c;

    cv::Mat kernelEI;
    cv::Mat kernelIE;
    //0: p=0 q=-1  1: p=0 q=1  2: p=-1 q=0  3: p=1 q=0
    //ask Alex Kondratyev for this code
    cv::Mat Nij[4];

    cv::Mat getDO(OPPONENT lm, OPPONENT ml);
    cv::Mat getWnew(const cv::Mat &Wold);
    void calcNpqij(const int scale);

    int scale = 0;

    const float A6 = 10.0f;
    const float B2 = 1.0f;
    const float C2 = 0.3f;
    const float A7 = 1.0f;
    const float Kp = 30.0f;
    const float eps = 0.0001f;
};


#endif //DPREEN_SEGMENTATION_REGIONENHANCEMENTSTAGE_H
