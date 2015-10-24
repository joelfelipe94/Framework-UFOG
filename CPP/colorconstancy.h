#ifndef COLORCONSTANCY_H
#define COLORCONSTANCY_H
#include "common.h"
class ColorConstancy
{
private:
    int method;
    Mat dcpPrior;
    Mat trans;


public:
    ColorConstancy();
    ColorConstancy(int input);
    void setDCP(Mat dcp);
    void setTrans(Mat dcp);
    Vec3f shadesOfGray(Mat image, int mikownsky);
    Vec3f colorLines(Mat image);
    Vec3f DCPveil(Mat image, Mat dcpImage);
    Vec3f getLightSource(Mat image);


};

#endif // COLORCONSTANCY_H
