//
// Created by kon on 19.03.18.
//

#include "Model.h"
#include "../Utility/Util.h"

Model::Model() :
    Retina(OpponentColorStage(SCALE)),
    V1(ChromaticContourStage()),
    V2(CompetitiveCooperativeStage())
{

}

const cv::Mat & Model::GetReOut() {
    return cv::Mat();
}

const std::map<OPPONENT, cv::Mat> & Model::GetOCOut(OPPONENT) {
    return Retina.GetOutput();
}

void Model::init(cv::Mat & sourceImg) {
    src = sourceImg;
}

cv::Mat Model::GetResult() {
    Process();
    //Util::printMat(out);
    return out;
}

void Model::Process() {
    //todo big cycle
    Retina.init(src);
    std::map<OPPONENT, cv::Mat> retinaOut = Retina.GetOutput();

    V1.init(retinaOut);
    cv::Mat ccsOut = V1.getStageOutput(SCALE, 3);
    //V1.getSi

    //tmp
    out = ccsOut;
}


