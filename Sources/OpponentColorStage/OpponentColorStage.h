#ifndef DPREEN_SEGMENTATION_OPPONENTCOLORSTAGE_H
#define DPREEN_SEGMENTATION_OPPONENTCOLORSTAGE_H


#include <opencv2/core/mat.hpp>
#include <map>
#include "../Model/IModel.h"

class OpponentColorStage {

public:
    explicit OpponentColorStage(int scale);

    cv::Mat getDRG();
    cv::Mat getDGR();
    cv::Mat getDBY();
    cv::Mat getDYB();
    cv::Mat getLuminanceON();
    cv::Mat getLuminanceOFF();

    std::map<OPPONENT, cv::Mat> GetOutput();

    //cv::Mat get

    void init(cv::Mat src);

    const cv::Mat getKernelEI();
    const cv::Mat getKernelIE();

private:


protected:
    float const A = 10.0;
    float const B = 3.0;
    float const C = 0.8;
    int const kernelSize = 7;
    cv::Mat red;
    cv::Mat green;
    cv::Mat blue;
    cv::Mat yellow;
    cv::Mat luminance;

    float sigmaE;
    float sigmaI;
    int scale;

    cv::Mat eGaussianKernel;
    cv::Mat iGaussianKernel;
    cv::Mat kernelIE;
    cv::Mat kernelEI;


    //May have parallel implementation:
    cv::Mat getEKernel();
    cv::Mat getIKernel();
    cv::Mat getResult(cv::Mat srcL, cv::Mat srcM);

};


#endif //DPREEN_SEGMENTATION_OPPONENTCOLORSTAGE_H
