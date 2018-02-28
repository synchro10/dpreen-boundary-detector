/*
 Non parallel implementation of OpponentColorStage interface
 */
#ifndef DPREEN_SEGMENTATION_OCS_H
#define DPREEN_SEGMENTATION_OCS_H


#include "interface/OpponentColorStage.h"

class OCS : public OpponentColorStage{
public:
    explicit OCS(int scale);

    cv::Mat getResult(cv::Mat srcL, cv::Mat srcM) override;

    cv::Mat getLuminanceOFF() override;

private:

    cv::Mat getEKernel() override;

    cv::Mat getIKernel() override;

};


#endif //DPREEN_SEGMENTATION_OCS_H
