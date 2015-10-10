#ifndef COLORCONSTANCY_H
#define COLORCONSTANCY_H
#include "common.h"
class ColorConstancy
{
public:
    ColorConstancy();
    Vec3f shadesOfGray(Mat image, int mikownsky);

    Vec3f getLightSource(Mat image);
};

#endif // COLORCONSTANCY_H
