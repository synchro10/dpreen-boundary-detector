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
    RegionEnhancementStage(std::shared_ptr<IModel> model);
    void init(std::vector<cv::Mat> U); //from V2
    std::map<OPPONENT, cv::Mat> getStageOut(const int scale);
private:
    std::shared_ptr<IModel> model;

    std::vector<cv::Mat> U;
    std::map<OPPONENT , cv::Mat> c;

    cv::Mat kernelEI;
    cv::Mat kernelIE;

    cv::Mat getDO(const int scale, OPPONENT lm, OPPONENT ml);
    cv::Mat getWnew(const int scale, const cv::Mat & Wold, OPPONENT lm);

    const float A6 = 10.0f;
    const float B2 = 1.0f;
    const float C2 = 0.3f;
    const float A7 = 1.0f;
    const float Kp = 30.0f;
    const float eps = 0.0001f;
};


#endif //DPREEN_SEGMENTATION_REGIONENHANCEMENTSTAGE_H
