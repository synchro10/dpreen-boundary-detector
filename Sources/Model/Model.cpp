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
    Iteration(0);
    int iterations = 1;
    bool isStop;
    const OPPONENT ch[4] = {OPPONENT::RG, OPPONENT::GR, OPPONENT::YB, OPPONENT::BY};
    while(!isStop && iterations != 10) {
        v4Out.swap(v4OutOld);
        Iteration(iterations);
        isStop = true;
        for(int i = 0; i < 4; i++){
            isStop &= Util::matLessScalar(cv::abs(v4Out[ch[i]] - v4OutOld[ch[i]]), 0.0001);
        }
        ++iterations;
    }
    std::cout << "iterations total: " << iterations << std::endl;

    //tmp
    //out = Util::perElementMax(v2Out);
    out = v4Out[OPPONENT::RG];

}

void Model::Iteration(const int iteration) {
    //Retina - Opponent color stage (4.1)
    Retina.init(src);
    retinaOut = Retina.GetOutput();

    //V1 - Chromatic contour stage (4.2)
    V1.init(retinaOut, this);
    v1Out = V1.getStageOutput(SCALE);

    for (auto & src : v1Out){
        //todo
        Util::normalize(src, 255.0, 0.0);
    }

    //V2 - Competitive cooperative stage (4.3)
    V2.init(v1Out);
    v2Out = V2.getStageOutput(SCALE);

    for (auto & src : v2Out){
        //todo
        Util::normalize(src, 255.0, 0.0);
    }

    //V4 - Region enhancement stage (4.4)
    V4.init(v2Out, SCALE, this);
    v4Out = V4.getStageOut();


#ifdef LOGGER
    {
        Util::saveImg(retinaOut[OPPONENT::RG], "RG", iteration, "retina");
        Util::saveImg(retinaOut[OPPONENT::GR], "GR", iteration, "retina");
        Util::saveImg(retinaOut[OPPONENT::YB], "YB", iteration, "retina");
        Util::saveImg(retinaOut[OPPONENT::BY], "BY", iteration, "retina");
    }
    for(int i = 0; i < 6; i++){
        Util::saveImg(v1Out[i], std::to_string(i), iteration, "v1");
    }
    for(int i = 0; i < 6; i++){
        Util::saveImg(v2Out[i], std::to_string(i), iteration, "v2");
    }
    {
        Util::saveImg(v4Out[OPPONENT::RG], "RG", iteration, "v4");
        Util::saveImg(v4Out[OPPONENT::GR], "GR", iteration, "v4");
        Util::saveImg(v4Out[OPPONENT::YB], "YB", iteration, "v4");
        Util::saveImg(v4Out[OPPONENT::BY], "BY", iteration, "v4");
    }
#endif
}

const cv::Mat & Model::getKernelEI() {
    return Retina.getKernelEI();
}

const cv::Mat & Model::getKernelIE() {
    return Retina.getKernelIE();
}


