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
    //todo
    return cv::Mat();
}

const std::map<OPPONENT, cv::Mat> & Model::GetOCOut() {
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

    //Retina - Opponent color stage (4.1)
    Retina.init(src);
    std::map<OPPONENT, cv::Mat> retinaOut = Retina.GetOutput();

    //V1 - Chromatic contour stage (4.2)
    V1.init(retinaOut);
    std::vector<cv::Mat> ccsOut = V1.getStageOutput(SCALE);

    //V2 - Competitive cooperative stage (4.3)
    V2.init(ccsOut);
    std::vector<cv::Mat> v2Out = V2.getStageOutput(SCALE);

    //tmp
    out = Util::perElementMax(ccsOut);
    //out = v2Out[5];

    //V4 - Region enhancement stage (4.4)

}
