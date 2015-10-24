#ifndef COMMON_H
#define COMMON_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <cv.h>
#include <dirent.h>

#include <eigen3/Eigen/Sparse>


#include <fstream>
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

void writeMatToFile(cv::Mat& m, const char* filename);
Mat fitSize(Mat image,int maxEdgeSize);
void writeImage8U(const cv::Mat& image,const char* filename);

void writeToColorMap(const Mat& image ,const char* filename);
// This is the 'elder trick of the...' - Tell the compiler this function is defined in other place
extern "C"
Mat solve_alpha(const Mat *img_cv, const Mat *img_scr_cv);

extern "C"
void free_vars();
extern "C"
void init_vars(int img_w, int img_h);


#define rowPtr(imagePtr, dataType, lineIndex) \
        (dataType *)(imagePtr->imageData + (lineIndex) * imagePtr->widthStep)


#endif // COMMON_H
