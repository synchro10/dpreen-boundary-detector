#ifndef DPREEN_SEGMENTATION_OPPONENTCOLORSTAGE_H
#define DPREEN_SEGMENTATION_OPPONENTCOLORSTAGE_H


#include <opencv2/core/mat.hpp>

class OpponentColorStage {

public:
    explicit OpponentColorStage(int scale);

    cv::Mat getDRG();
    cv::Mat getDGR();
    cv::Mat getDBY();
    cv::Mat getDYB();
    cv::Mat getLuminanceON();
    virtual cv::Mat getLuminanceOFF() = 0;

    //cv::Mat get

    void init(cv::Mat src);

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
    virtual cv::Mat getEKernel() = 0;
    virtual cv::Mat getIKernel() = 0;
    virtual cv::Mat getResult(cv::Mat srcL, cv::Mat srcM) = 0;

};


#endif //DPREEN_SEGMENTATION_OPPONENTCOLORSTAGE_H
