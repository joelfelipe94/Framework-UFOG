#ifndef L0SMOOTHING_H
#define L0SMOOTHING_H
#include "common.h"
class L0smoothing
{
public:
    L0smoothing();
    Mat doFilter(Mat image);
};

#endif // L0SMOOTHING_H
