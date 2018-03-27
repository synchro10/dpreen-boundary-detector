//
// Created by kon on 19.03.18.
//

#include "Model.h"
#include "../Utility/Util.h"
#include <memory>

Model::Model() :
    Retina(OpponentColorStage(SCALE)),
    V1(ChromaticContourStage()),
    V2(CompetitiveCooperativeStage()),
    V4(RegionEnhancementStage())
{

}

const std::map<OPPONENT, cv::Mat> & Model::GetReOut() {
    return v4OutOld;
}

const std::map<OPPONENT, cv::Mat> & Model::GetOCOut() {
    return retinaOut;
}

void Model::init(cv::Mat & sourceImg) {
    src = sourceImg;

    //todo check
    v4Out[OPPONENT::RG] = cv::Mat(src.rows, src.cols, src.type(), 0.f);
    v4Out[OPPONENT::GR] = cv::Mat(src.rows, src.cols, src.type(), 0.f);
    v4Out[OPPONENT::BY] = cv::Mat(src.rows, src.cols, src.type(), 0.f);
    v4Out[OPPONENT::YB] = cv::Mat(src.rows, src.cols, src.type(), 0.f);
    v4OutOld = v4Out;
}

cv::Mat Model::GetResult() {
    Process();
    return out;
}

void Model::Process() {
    Iteration();
    int iterations = 1;
    bool isStop;
    const OPPONENT ch[4] = {OPPONENT::RG, OPPONENT::GR, OPPONENT::YB, OPPONENT::BY};
    while(!isStop && iterations != 28) {
        v4Out.swap(v4OutOld);
        Iteration();
        isStop = true;
        for(int i = 0; i < 4; i++){
            isStop &= Util::matLessScalar(cv::abs(v4Out[ch[i]] - v4OutOld[ch[i]]), 0.0001);
        }
        ++iterations;
    }
    std::cout << "iterations total: " << iterations << std::endl;

    for(int i = 0; i < 6; i++){
        Util::printMat(v2Out[i], "v2 " + std::to_string(i));
    }
    {
        Util::printMat(v4Out[OPPONENT::RG], "v4 RG");
        Util::printMat(v4Out[OPPONENT::GR], "v4 GR");
        Util::printMat(v4Out[OPPONENT::YB], "v4 YB");
        Util::printMat(v4Out[OPPONENT::BY], "v4 BY");
    }

    //tmp
    //out = Util::perElementMax(v2Out);
    out = v4Out[OPPONENT::RG];

}

void Model::Iteration() {
    //Retina - Opponent color stage (4.1)
    Retina.init(src);
    retinaOut = Retina.GetOutput();

    //V1 - Chromatic contour stage (4.2)
    V1.init(retinaOut, this);
    v1Out = V1.getStageOutput(SCALE);

    //V2 - Competitive cooperative stage (4.3)
    V2.init(v1Out);
    v2Out = V2.getStageOutput(SCALE);

    //V4 - Region enhancement stage (4.4)
    V4.init(v2Out, SCALE, this);
    v4Out = V4.getStageOut();
}

const cv::Mat & Model::getKernelEI() {
    return Retina.getKernelEI();
}

const cv::Mat & Model::getKernelIE() {
    return Retina.getKernelIE();
}


