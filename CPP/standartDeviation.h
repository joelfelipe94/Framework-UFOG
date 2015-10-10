#ifndef STANDARDEVIATION_H

#define STANDARDEVIATION_H


#include "common.h"


class StandardDeviation
{
public:
    StandardDeviation();
    Mat computePrior(Mat imagemOriginal,int n);

};

#endif // STANDARDEVIATION_H
