
#include "common.h"
#include "l0smoothing.h"
#include "signalprior.h"
#include "darkchannelprior.h"
#include "colorconstancy.h"
#include "standartDeviation.h"

Mat getHistSP(char * imageName){


// For now we are using just an image after u can put the folder to process.

Mat image;

cout << imageName << endl;

image = imread(imageName, CV_LOAD_IMAGE_COLOR);
image.convertTo(image, CV_32FC3);
image = image/255;

//cout << image << endl;


L0smoothing filter;
SignalPrior sp;

//image = filter.doFilter(image);
//Mat imageR,imageG,imageB;
ColorConstancy cp;

Vec3f veil = cp.getLightSource(image);
cout << veil << endl;
cv::resize(image,image,Size(),0.5,0.5);
Mat transmission = sp.computePrior(image,15,veil);


cout << " the end " << endl;
//cout << transmission << endl;
//transmission = transmission;
transmission = transmission*255;

 //cout << transmission << endl;

transmission.convertTo(transmission,CV_8U);
imwrite("trans.png",transmission);
return  computeHistogram(transmission);

//cout << hist << endl;




}
Mat getHistDCP(char * imageName){


// For now we are using just an image after u can put the folder to process.

Mat image;

cout << imageName << endl;

image = imread(imageName, CV_LOAD_IMAGE_COLOR);
image.convertTo(image, CV_64FC3);
image = image/255;

//cout << image << endl;


L0smoothing filter;
DarkChannelPrior sp;


//image = filter.doFilter(image);
//Mat imageR,imageG,imageB;

cv::resize(image,image,Size(),0.5,0.5);
Mat transmission = sp.computePrior(image,15);


cout << " the end " << endl;
//cout << transmission << endl;
//transmission = transmission;
transmission = transmission*255;

 //cout << transmission << endl;

transmission.convertTo(transmission,CV_8U);
imwrite("trans.png",transmission);
return  computeHistogram(transmission);

//cout << hist << endl;




}

Mat getHistSTD(char * imageName){


// For now we are using just an image after u can put the folder to process.

Mat image;

cout << imageName << endl;

image = imread(imageName, CV_LOAD_IMAGE_COLOR);
image.convertTo(image, CV_64FC3);
image = image/255;

//cout << image << endl;


L0smoothing filter;
StandardDeviation sp;


//image = filter.doFilter(image);
//Mat imageR,imageG,imageB;

cv::resize(image,image,Size(),0.5,0.5);
Mat transmission = sp.computePrior(image,15);


cout << " the end " << endl;
//cout << transmission << endl;
//transmission = transmission;
transmission = transmission*255;

 //cout << transmission << endl;

transmission.convertTo(transmission,CV_8U);
imwrite("trans.png",transmission);
return  computeHistogram(transmission);

//cout << hist << endl;




}

int runDataset(char * datasetPath)
{

    DIR * dirp;
    dirp = opendir(datasetPath);
    if (dirp == NULL)
            return (1);

    dirent * dp;
    dp = readdir(dirp);
    dp = readdir(dirp);
    int N =150;
    int count = 0;
    Mat averageHist = Mat::zeros(256,1,CV_32F);
    Mat hist;
    while ((dp = readdir(dirp)) != NULL && count < N) {


        cout << dp->d_name << endl;
        cout << count << endl;
        char fullPath[256];
        sprintf(fullPath,"%s/%s",datasetPath,dp->d_name);
        hist = getHistDCP(fullPath);
        cout << averageHist.rows << " " << hist.rows << endl;
        cout << hist.type() << endl;
        averageHist =  hist + averageHist;
        cout << " SUMOU " << endl;
        count++;

   }
    averageHist = averageHist/N;
    cout << averageHist << endl;
    drawHist(averageHist);


    return 0;


}


int main(int argc, char* argv[])
{
    printf("densecrf \n");


    cout <<  argc << endl;
    if (argc<2){
       printf("Usage: %s image annotations output\n", argv[0] );
       return 1;
    }
    Mat hist;
    hist = getHistSTD(argv[1]);
    //runDataset(argv[1]);

    return 0;

}

