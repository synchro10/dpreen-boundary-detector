//
// Created by kon on 19.03.18.
//

#ifndef DPREEN_SEGMENTATION_IARCHITECTURE_H
#define DPREEN_SEGMENTATION_IARCHITECTURE_H

#include <opencv2/core/mat.hpp>
#include <map>

/**
 * 6 оппонентов
 */
enum OPPONENT{
    RG, GR, YB, BY, ON, OFF
};

/**
 * тут интерфейс (для разрешения цикл. зависимости)
 * который предоставит общение между слоями
 */
class IModel {
public:
    virtual const cv::Mat & GetReOut() = 0;
    virtual const std::map<OPPONENT, cv::Mat> & GetOCOut(OPPONENT) = 0;
};


#endif //DPREEN_SEGMENTATION_IARCHITECTURE_H
