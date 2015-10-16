#ifndef STANDARDEVIATIONPRIOR_H

#define STANDARDEVIATIONPRIOR_H


#include "common.h"


class StandartDeviationPrior
{
public:
    StandartDeviationPrior();
    Mat computePrior(Mat imagemOriginal,int n);
    Mat computeTransmission(Mat imagemOriginal,int n);

};

#endif // STANDARDEVIATIONPRIOR_H
