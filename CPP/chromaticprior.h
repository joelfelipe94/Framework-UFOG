#ifndef CHROMATICPRIOR_H
#define CHROMATICPRIOR_H
#include "common.h"
class ChromaticPrior
{
public:
    double prior(Mat tile,Vec3f veil);
    Mat computePrior(Mat image, int patchsize, Vec3f veil);
    Mat computeTransmission(Mat imageBilateral , int patchsize , Vec3f veil);
};

#endif // CHROMATICPRIOR_H
