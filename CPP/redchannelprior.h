#ifndef REDCHANNELPRIOR_H
#define REDCHANNELPRIOR_H
#include "common.h"
class RedChannelPrior
{
public:
    RedChannelPrior();
    double prior(Mat tile);


    Mat computePrior(Mat image, int patchsize);
    Mat computeTransmission(Mat image, int patchsize, Vec3f veil);
};

#endif // REDCHANNELPRIOR_H
