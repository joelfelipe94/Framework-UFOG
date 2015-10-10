#include "common.h"

#ifndef SIGNALPRIOR_H
#define SIGNALPRIOR_H

class SignalPrior
{
public:
    SignalPrior();
    Mat computePrior(Mat image, int patchsize , Vec3f Binf);
    double prior(Mat tile,Vec3f veil);
    double priorBinf(Mat tile,Vec3f veil);
    Mat computeTransmission(Mat image, int patchsize , Vec3f veil);

};

#endif // SIGNALPRIOR_H
