#ifndef DCPGB_H
#define DCPGB_H
#include "common.h"

class DCPGB
{
public:
    DCPGB();
    double prior(Mat tile);
    Mat computePrior(Mat image, int patchsize);
    Mat computeTransmission(Mat image, int patchsize, Vec3f veil);
};

#endif // DCPGB_H
