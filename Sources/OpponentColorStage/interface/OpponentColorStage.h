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

    void init(cv::Mat src);

protected:
    float const A = 10.0;
    float const B = 3.0;
    float const C = 0.8;
    int const kernelSize = 7;
    cv::Mat red;
    cv::Mat green;
    cv::Mat blue;
    cv::Mat yellow;

    float sigmaE;
    float sigmaI;
    int scale;

    cv::Mat eGaussianKernel;
    cv::Mat iGaussianKernel;

    //May have parallel implementation:
    virtual cv::Mat getEKernel() = 0;
    virtual cv::Mat getIKernel() = 0;
    virtual cv::Mat getRezult(cv::Mat srcL, cv::Mat srcM) = 0;

};


#endif //DPREEN_SEGMENTATION_OPPONENTCOLORSTAGE_H
