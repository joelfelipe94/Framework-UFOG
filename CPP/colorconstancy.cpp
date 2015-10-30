#include "colorconstancy.h"
#include "colorlines.h"


ColorConstancy::ColorConstancy()
{
    method =2;

}
ColorConstancy::ColorConstancy(int input)
{
    method = input;
}

void ColorConstancy::setDCP(Mat dcp)
{
    dcpPrior = dcp;
}
void ColorConstancy::setTrans(Mat transmission)
{
    trans = transmission;
}


Vec3f ColorConstancy::shadesOfGray(const Mat& image, int mikownsky)
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

Vec3f ColorConstancy::colorLines(const Mat& image)
{

    ColorLines cl;

    Vec3f ori = cl.findOrientation(&image);
    cout << "ori" << endl;
    cout << ori << endl;
    double a[3]={ori[0],ori[1],ori[2]};

    //cout << image << endl;
    double  mag =  cl.findMagnitude(&image,&trans,a);
    cout << mag << endl;

    return ori*mag;


}

Vec3f ColorConstancy::maxRedChannel(const Mat &image)
{

    double minValR;


    Point minLoc;
    Point maxLoc;
    vector<Mat> channels;



    split(image,channels);






    minMaxLoc( 1- channels[0], &minValR, &minValR, &minLoc, &maxLoc );

    //cout << maxValR << " " << maxValG << " " << maxValB << endl;

    return image.at<Vec3f>(maxLoc);

}

Vec3f ColorConstancy::DCPveil(const Mat& image, Mat dcpImage)
{

    Mat dst;



    dcpImage = dcpImage.reshape(0,1);



    sortIdx(dcpImage, dst, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
    float maxValue = 0;
    vector<Mat> channels;
    split(image,channels);
    Vec3f veil;

    //cout << dst.rows  << " " << dst.cols << endl;
    //cout << " chnnels" << image.channels() << " " << endl;
    channels[0] = channels[0].reshape(0,1);
    channels[1] = channels[1].reshape(0,1);
    channels[2] = channels[2].reshape(0,1);


    //cout << channels[0].rows  << " " << channels[0].cols << endl;
    /// Take the 10% smaller
    for (int i=0;i<int((image.rows*image.cols)/10);i++){
        float intensity = 0.0;


        intensity = intensity + channels[0].at<float>(0,dst.at<int>(0,i));
        intensity = intensity +channels[1].at<float>(0,dst.at<int>(0,i));
        intensity = intensity +channels[2].at<float>(0,dst.at<int>(0,i));


        if (maxValue < intensity){
            veil[0] = channels[0].at<float>(0,dst.at<int>(0,i));

            veil[1] = channels[1].at<float>(0,dst.at<int>(0,i));

            veil[2] = channels[2].at<float>(0,dst.at<int>(0,i));
            maxValue = intensity;
        }

    }

        return veil;


}

Vec3f ColorConstancy::getLightSource(const Mat& image)
{

    switch(method){

        case 1:
        /// Selected max RGB
           return shadesOfGray(image,1000);
        break;
        case 2:
        /// Selected shades of Gray
          return shadesOfGray(image,6);

        break;

        case 3:
        /// Selected DCP prior;
        ///
            if (dcpPrior.isContinuous()){
            return DCPveil(image,dcpPrior);
            }
            else{
                cout << " ERRROR " << endl;
                return Vec3f();
            }

        break;
        case 4:

            return colorLines(image);

        break;
        case 5:

        return maxRedChannel(image);

        break;


    default:

        break;

    }
}
