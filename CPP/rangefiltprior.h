#ifndef RANGEFILT_H

#define RANGEFILT_H
#include <iostream>
#include <vector>
#include "cv.h"
#include "highgui.h"
#include <omp.h>
using namespace cv;
using namespace std;

class RangeFiltPrior
{
public:
    double prior(Mat tile,Vec3f veil);
    Mat computePrior(Mat image, int patchsize, Vec3f veil);
    Mat computeTransmission(Mat image, int patchsize, Vec3f veil);
};




#endif // RANGEFILT_H
