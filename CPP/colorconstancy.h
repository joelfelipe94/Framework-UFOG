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
    Vec3f shadesOfGray(const Mat& image, int mikownsky);
    Vec3f colorLines(const Mat& image);
    Vec3f maxRedChannel(const Mat& image);
    Vec3f DCPveil(const Mat& image, Mat dcpImage);
    Vec3f getLightSource(const Mat& image);


};

#endif // COLORCONSTANCY_H
