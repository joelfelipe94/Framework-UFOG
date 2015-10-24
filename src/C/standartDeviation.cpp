#include <iostream>

#include "cv.h"
#include "highgui.h"
using namespace cv;
Mat mat2gray(const Mat& src)
{
    Mat dst;
    normalize(src, dst, 0.0, 1.0, NORM_MINMAX);
    return dst;
}
int main( int argc, char* argv[] )
{
    Mat image = imread("../../Imagens/Photo3D/1.jpg", 0);

     Mat image32f;
     image.convertTo(image32f, CV_32F);

     Mat mu;
     blur(image32f, mu, Size(3, 3));

     Mat mu2;
     blur(image32f.mul(image32f), mu2, Size(3, 3));

     Mat sigma;
     sqrt(mu2 - mu.mul(mu), sigma);
     namedWindow("original", WINDOW_NORMAL);
     imshow("original", mat2gray(image32f));
     imshow("mu", mat2gray(mu));
     imshow("sigma",mat2gray(sigma));
     waitKey();
     return 0;

}
