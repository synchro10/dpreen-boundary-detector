//
// Created by kon on 19.03.18.
//

#ifndef DPREEN_SEGMENTATION_ARCHITECTURE_H
#define DPREEN_SEGMENTATION_ARCHITECTURE_H


#include "IModel.h"
#include "../OpponentColorStage/OpponentColorStage.h"
#include "../ChromaticContourStage/ChromaticContourStage.h"
#include "../CompetitiveCooperativeStage/CompetitiveCooperativeStage.h"
#include "../GlobalConstants.h"
#include "../RegionEnhancementStage/RegionEnhancementStage.h"

#define SCALE 1
//#define EPS 0.0001

class Model : public IModel{
public:
    Model();

    const cv::Mat & GetReOut() override;
    const std::map<OPPONENT, cv::Mat> & GetOCOut() override;
    const cv::Mat getKernelEI() override;
    const cv::Mat getKernelIE() override ;

    void init(cv::Mat & sourceImg);
    cv::Mat GetResult();

private:
    cv::Mat src;
    cv::Mat out;

    OpponentColorStage Retina;
    ChromaticContourStage V1;
    CompetitiveCooperativeStage V2;
    RegionEnhancementStage V4;

    void Process();
};


#endif //DPREEN_SEGMENTATION_ARCHITECTURE_H
