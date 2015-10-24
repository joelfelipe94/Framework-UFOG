#ifndef COLORLINES_H
#define COLORLINES_H
#include "common.h"



using namespace Eigen;

class ColorLines
{


public:
    ColorLines();

    Vec3f findOrientation(Mat *m);
    double findMagnitude(Mat *img, Mat *alpha_mat, double a[3]);
private:

    void patchLine(double lambdas[3], double p1[3], double p2[3], double *img, int h, int w, int c, int min_y, int max_y, int min_x, int max_x, bool onlyPositive);
    void my_sort(double *v, int *inds,int num_e);
};

#endif // COLORLINES_H
