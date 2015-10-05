#ifndef DARKCHANNELPRIOR_H
#define DARKCHANNELPRIOR_H
#include "common.h"
class DarkChannelPrior
{
public:
    DarkChannelPrior();
    double dcp(Mat tile);

    Mat computePrior(Mat image, int patchsize);
};

#endif // DARKCHANNELPRIOR_H
