//
// Created by kon on 25.03.18.
//

#include <opencv/cv.hpp>
#include "RegionEnhancementStage.h"
#include "../Utility/Util.h"

RegionEnhancementStage::RegionEnhancementStage(std::shared_ptr<IModel> model) {
    this->model = model;
    //get c from (4)
    this->c = std::move(model->GetOCOut());
    //get kernels from (5)
    this->kernelIE = std::move(model->getKernelIE());
    this->kernelEI = std::move(model->getKernelEI());
}

void RegionEnhancementStage::init(std::vector<cv::Mat> U) {
    this->U = std::move(U);
}

std::map<OPPONENT, cv::Mat> RegionEnhancementStage::getStageOut(const int scale) {
    std::map<OPPONENT, cv::Mat> out;
    OPPONENT lm[4] = {OPPONENT::RG, OPPONENT::GR, OPPONENT::BY, OPPONENT::YB};
    OPPONENT ml[4] = {OPPONENT::GR, OPPONENT::RG, OPPONENT::YB, OPPONENT::BY};
    for (int i = 0; i < 4; i++){
        cv::Mat DO = getDO(scale, lm[i], ml[i]);
        cv::Mat Wnew = getWnew(scale, DO, lm[i]);
        cv::Mat Wold = DO;
        int iterations = 1;
        while(Util::matLessScalar(cv::abs(Wnew - Wold), eps)){
            cv::Mat tmp = Wnew;
            Wnew = getWnew(scale, Wold, lm[i]);
            Wold = std::move(tmp);
            iterations++;
        }
        out[lm[i]] = Wnew;
        std::cout << "V4 iterations = " << iterations << std::endl;
    }
    return out;
}

//according to (14)
cv::Mat RegionEnhancementStage::getDO(const int scale, OPPONENT lm, OPPONENT ml) {
    cv::Mat filtredLM, filtredML;
    cv::filter2D(c[lm], filtredLM, c[lm].depth(), kernelEI);
    cv::filter2D(c[ml], filtredML, c[ml].depth(), kernelIE);

    cv::Mat out = (B2 * filtredLM - C2 * filtredML) / (A6 + filtredLM + filtredML);
    return std::move(out);
}

//according to (15)
cv::Mat RegionEnhancementStage::getWnew(const int scale, const cv::Mat &Wold, OPPONENT lm) {
    //todo
    return cv::Mat();
}
