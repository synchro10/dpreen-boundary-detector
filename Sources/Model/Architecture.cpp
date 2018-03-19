//
// Created by kon on 19.03.18.
//

#include "Architecture.h"

Architecture::Architecture() :
    Retina(OpponentColorStage(1)),
    V1(ChromaticContourStage()),
    V2(CompetitiveCooperativeStage())
{

}

const cv::Mat &Architecture::GetReOut() {
    return cv::Mat();
}

const cv::Mat &Architecture::GetOCOut(OPPONENT) {
    return cv::Mat();
}

void Architecture::init(cv::Mat &sourceImg) {

}


