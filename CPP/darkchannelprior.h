#ifndef DARKCHANNELPRIOR_H
#define DARKCHANNELPRIOR_H
#include "common.h"
class DarkChannelPrior
{
public:
    DarkChannelPrior();
    double dcp(Mat tile);


    Mat computePrior(Mat image, int patchsize);
    Mat computeTransmission(Mat image, int patchsize, Vec3f veil);
};

#endif // DARKCHANNELPRIOR_H
