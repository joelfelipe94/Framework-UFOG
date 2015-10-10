#include "colorconstancy.h"

ColorConstancy::ColorConstancy()
{
}

Vec3f ColorConstancy::shadesOfGray(Mat image, int mikownsky)
{

    /// sum  all the image pixels


    double sumR =0;
    double sumG =0;
    double sumB =0;
    for (int i=0;i<image.rows;i++){
        for (int j=0;j<image.cols;j++){

            //cout << "image R" << image.at<Vec3f>(i,j)[0] << endl;
            sumR += pow(image.at<Vec3f>(i,j)[0],double(mikownsky));
            sumG += pow(image.at<Vec3f>(i,j)[1],double(mikownsky));
            sumB += pow(image.at<Vec3f>(i,j)[2],double(mikownsky));


        }

    }


    cout << " Sums " << sumR << " " << sumG << " " << sumB << endl;
    sumR = sumR/(image.rows*image.cols);
    sumR = pow(sumR,double(1.0/double(mikownsky)));


    sumG = sumG/(image.rows*image.cols);
    sumG = pow(sumG,double(1.0/double(mikownsky)));


    sumB = sumB/(image.rows*image.cols);
    sumB = pow(sumB,double(1.0/double(mikownsky)));

    Vec3f source(sumR,sumG,sumB);
    return source;

}

Vec3f ColorConstancy::getLightSource(Mat image)
{

    return shadesOfGray(image,6);
}
