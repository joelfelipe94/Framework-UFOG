#ifndef COMMON_H
#define COMMON_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <dirent.h>

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <omp.h>


using namespace cv;
using namespace std;
// TODO: maybe return a true or false as a result

void fft(Mat I, Mat & amp, Mat & phase);

Mat  computeHistogram(Mat image);
void drawHist(Mat hist);


#define rowPtr(imagePtr, dataType, lineIndex) \
        (dataType *)(imagePtr->imageData + (lineIndex) * imagePtr->widthStep)


#endif // COMMON_H
