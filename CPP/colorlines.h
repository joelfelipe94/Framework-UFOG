#ifndef COLORLINES_H
#define COLORLINES_H
#include "common.h"

class ColorLines
{
public:
    ColorLines();
    Vec3f findOrientation(Mat image);
};

#endif // COLORLINES_H
