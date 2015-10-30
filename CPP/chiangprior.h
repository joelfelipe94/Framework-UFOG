#ifndef CHIANGPRIOR_H
#define CHIANGPRIOR_H
#include "common.h"
class ChiangPrior
{

    Vec3f Nrer;
public:
    ChiangPrior();
    double prior(Mat tile);


    Mat computePrior(Mat image, int patchsize);
    Mat computeTransmission(Mat image, int patchsize, Vec3f veil);

    double findDepth(Vec3f airlight,Vec3f ambientlight);
    Mat generateDepthImage(Mat image, Mat trans);
};

#endif // CHIANGPRIOR_H
