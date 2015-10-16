#ifndef GRADIENTES1_H
#define GRADIENTES1_H
#include "common.h"

#define SIGN(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))


class GradientS1Prior
{
private:
    double PYTHAG(double a, double b);
    int dsvd(double a[][2], int m, int n, double w[2], double v[][2]);
public:
    double prior(Mat tile);
    Mat computePrior(Mat image, int patchsize);
    Mat computeTransmission(Mat image, int patchsize);
};

#endif // GRADIENTES1_H
