//
// Created by kon on 19.03.18.
//

#ifndef DPREEN_SEGMENTATION_ARCHITECTURE_H
#define DPREEN_SEGMENTATION_ARCHITECTURE_H


#include "IArchitecture.h"
#include "../OpponentColorStage/OpponentColorStage.h"
#include "../ChromaticContourStage/ChromaticContourStage.h"
#include "../CompetitiveCooperativeStage/CompetitiveCooperativeStage.h"

class Architecture : public IArchitecture{
public:
    Architecture();

    const cv::Mat & GetReOut() override;
    const cv::Mat & GetOCOut(OPPONENT) override;

    void init(cv::Mat & sourceImg);

private:
    OpponentColorStage Retina;
    ChromaticContourStage V1;
    CompetitiveCooperativeStage V2;

    //todo
    //RegionEnhancementStage V4;
};


#endif //DPREEN_SEGMENTATION_ARCHITECTURE_H
