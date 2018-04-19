//
// Created by kon on 25.03.18.
//

#include <opencv/cv.hpp>
#include "RegionEnhancementStage.h"
#include "../Utility/Util.h"

RegionEnhancementStage::RegionEnhancementStage() {

}

void RegionEnhancementStage::init(std::vector<cv::Mat> U, const int scale, IModel *model) {
    this->model = model;
    //get c from (4)
    this->c = std::move(model->GetOCOut());
    //get kernels from (5)
    this->kernelIE = std::move(model->getKernelIE());
    this->kernelEI = std::move(model->getKernelEI());
    this->scale= scale;
    this->U = std::move(U);
    calcNpqij(scale);
}

std::map<OPPONENT, cv::Mat> RegionEnhancementStage::getStageOut() {
    std::map<OPPONENT, cv::Mat> out;
    OPPONENT lm[4] = {OPPONENT::RG, OPPONENT::GR, OPPONENT::BY, OPPONENT::YB};
    OPPONENT ml[4] = {OPPONENT::GR, OPPONENT::RG, OPPONENT::YB, OPPONENT::BY};
    for (int i = 0; i < 4; i++){
        cv::Mat DO = getDO(lm[i], ml[i]);
        cv::Mat Wold = DO;
        cv::Mat Wnew = getWnew(Wold);
        int iterations = 1;
        while(!Util::matLessScalar(cv::abs(Wnew - Wold), eps)){
            cv::Mat tmp = Wnew;
            Wnew = getWnew(Wold);
            Wold = std::move(tmp);
            iterations++;
        }
        out[lm[i]] = Wnew;
        std::cout << "V4 iterations = " << iterations << std::endl;
    }
    return out;
}

//according to (14)
cv::Mat RegionEnhancementStage::getDO(OPPONENT lm, OPPONENT ml) {
    cv::Mat filtredLM, filtredML;
    cv::filter2D(c[lm], filtredLM, c[lm].depth(), kernelEI);
    cv::filter2D(c[ml], filtredML, c[ml].depth(), kernelIE);

    cv::Mat out = (B2 * filtredLM - C2 * filtredML) / (A6 + filtredLM + filtredML);
    return std::move(out);
}

//according to (15)
cv::Mat RegionEnhancementStage::getWnew(const cv::Mat &Wold) {
    cv::Mat Wnew = cv::Mat(Wold.rows, Wold.cols, Wold.type(), 0.f);
    for (int i = 0; i < Wnew.rows; ++i){
        for (int j = 0; j < Wnew.cols; ++j){
            float filtred1 = Nij[0].at<float>(i, j) * Wold.at<float>(i, j - 1) +
                    Nij[1].at<float>(i, j) * Wold.at<float>(i, j + 1) +
                    Nij[2].at<float>(i, j) * Wold.at<float>(i - 1, j) +
                    Nij[3].at<float>(i, j) * Wold.at<float>(i + 1, j);
            float filtred2 = Nij[0].at<float>(i, j) + Nij[1].at<float>(i, j) + Nij[2].at<float>(i, j) + Nij[3].at<float>(i, j);
            Wnew.at<float>(i, j) = (Wold.at<float>(i, j) + filtred1) / (A7 + filtred2);
        }
    }
    return std::move(Wnew);
}

void RegionEnhancementStage::calcNpqij(const int scale) {
    Nij[0] = cv::Mat(U[0].rows, U[0].cols, U[0].type(), 0.f);
    Nij[1] = cv::Mat(U[0].rows, U[0].cols, U[0].type(), 0.f);
    Nij[2] = cv::Mat(U[0].rows, U[0].cols, U[0].type(), 0.f);
    Nij[3] = cv::Mat(U[0].rows, U[0].cols, U[0].type(), 0.f);

    cv::Mat Uij = cv::Mat(U[0].rows, U[0].cols, U[0].type(), 0.f);
    for(const auto & Uk : U){
        Uij = Uij + Uk;
    }

    for (int i = 0; i < Uij.rows; ++i){
        for (int j = 0; j < Uij.cols; ++j){
            Nij[0].at<float>(i, j) = j != 0 ? expf(-1.f*Kp*(Uij.at<float>(i, j) + Uij.at<float>(i, j - 1)))
                                            : expf(-1.f*Kp*(Uij.at<float>(i, j)));
            Nij[1].at<float>(i, j) = j != Nij[0].cols - 1 ? expf(-1.f*Kp*(Uij.at<float>(i, j) + Uij.at<float>(i, j + 1)))
                                            : expf(-1.f*Kp*(Uij.at<float>(i, j)));
            Nij[2].at<float>(i, j) = i != 0 ? expf(-1.f*Kp*(Uij.at<float>(i, j) + Uij.at<float>(i - 1, j)))
                                            : expf(-1.f*Kp*(Uij.at<float>(i, j)));
            Nij[3].at<float>(i, j) = i != Nij[0].rows - 1 ? expf(-1.f*Kp*(Uij.at<float>(i, j) + Uij.at<float>(i + 1, j)))
                                                          : expf(-1.f*Kp*(Uij.at<float>(i, j)));
        }
    }


}
