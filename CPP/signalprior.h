#ifndef SIGNALPRIOR_H
#define SIGNALPRIOR_H
#include "common.h"
class SignalPrior
{
public:
    double prior(Mat tile, Vec3f veil);
    Mat computePrior(Mat imageBilateral , Mat image, int patchsize, Vec3f veil);
    Mat computeTransmission(cv::Mat imageBilateral, Mat image, int patchsize , Vec3f veil);
};

#endif // SIGNALPRIOR_H
