#include "common.h"

#ifndef SIGNALPRIOR_H
#define SIGNALPRIOR_H

class SignalPrior
{
public:
    SignalPrior();
    Mat computePrior(Mat image, int patchsize , Vec3d Binf);
    double prior(Mat tile,Vec3d veil);
    double dcp(Mat tile,Vec3d veil);
};

#endif // SIGNALPRIOR_H
