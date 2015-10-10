
#include "common.h"
#include "l0smoothing.h"
#include "signalprior.h"
#include "darkchannelprior.h"
#include "colorconstancy.h"
#include "standartDeviation.h"
#include "bilateralfilter.h"
#include "restoration.h"

Mat getHistSP(char * imageName){


// For now we are using just an image after u can put the folder to process.

Mat image, image_out;

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


const double sigmaColor = 100.0;
    const double sigmaSpace = 10.0;
    cvtColor(image, image, CV_RGB2Lab);
    vector<Mat> channels;
split(image,channels);
cv_extend::bilateralFilter(channels[0], channels[0], sigmaColor, sigmaSpace);

merge(channels,image);
cvtColor(image, image, CV_Lab2RGB);
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


Mat restoreSP(char * imageName){


// For now we are using just an image after u can put the folder to process.

    Mat image, image_out;

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


    const double sigmaColor = 100.0;
        const double sigmaSpace = 10.0;
        cvtColor(image, image_out, CV_BGR2Lab);
        vector<Mat> channels;
    split(image_out,channels);
    cv_extend::bilateralFilter(channels[0], channels[0], sigmaColor, sigmaSpace);

    merge(channels,image_out);
    cvtColor(image_out, image_out, CV_Lab2RGB);
     cvtColor(image, image, CV_BGR2RGB);
    Vec3f veil = cp.getLightSource(image);
    cout << veil << endl;
    //cv::resize(image,image,Size(),0.5,0.5);

    //veil[0] = 95.0/255.0;
    //veil[1] = 170.0/255.0;
    //veil[2] = 127.0/255.0;

    Mat transmission = sp.computePrior(image_out,7,veil);

    transmission.convertTo(transmission,CV_32F);


    cout << " the end " << endl;
    //cout << transmission << endl;
    //transmission = transmission;
    Mat tImage = transmission*255;
    tImage.convertTo(tImage,CV_8U);
    imwrite("trans.png",tImage);

     //cout << transmission << endl;

    //


    Restoration restore;
    return restore.restoreImageClassic(image, transmission,  veil);


    //cout << hist << endl;




}


Mat restoreDCP(char * imageName){


// For now we are using just an image after u can put the folder to process.

    Mat image, image_out;

    cout << imageName << endl;

    image = imread(imageName, CV_LOAD_IMAGE_COLOR);
    image.convertTo(image, CV_32FC3);
    image = image/255;

    //cout << image << endl;


    DarkChannelPrior sp;

    //image = filter.doFilter(image);
    //Mat imageR,imageG,imageB;
    ColorConstancy cp;


    const double sigmaColor = 100.0;
        const double sigmaSpace = 10.0;
        cvtColor(image, image, CV_RGB2Lab);
        vector<Mat> channels;
    split(image,channels);
    cv_extend::bilateralFilter(channels[0], channels[0], sigmaColor, sigmaSpace);

    merge(channels,image);
    cvtColor(image, image, CV_Lab2RGB);
    Vec3f veil = cp.getLightSource(image);
    cout << veil << endl;
    //cv::resize(image,image,Size(),0.5,0.5);
    Mat transmission = sp.computeTransmission(image,7,veil);

    transmission.convertTo(transmission,CV_32F);


    cout << " the end " << endl;
    Mat tImage = transmission*255;
    tImage.convertTo(tImage,CV_8U);
    imwrite("trans.png",tImage);

    //cout << transmission << endl;
    //transmission = transmission;
    //transmission = transmission*255;

     //cout << transmission << endl;

    //transmission.convertTo(transmission,CV_8U);


    Restoration restore;
    return restore.restoreImageClassic(image, transmission,  veil);


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
    int N =500;
    int count = 0;
    Mat averageHist = Mat::zeros(256,1,CV_32F);
    Mat hist;
    while ((dp = readdir(dirp)) != NULL && count < N) {


        cout << dp->d_name << endl;
        cout << count << endl;
        char fullPath[256];
        sprintf(fullPath,"%s/%s",datasetPath,dp->d_name);
        hist = getHistSP(fullPath);
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
    //hist = runDataset(argv[1]);
    //imwrite("hist.png",hist);

    hist = getHistSTD(argv[1]);

//    Mat final;
//    final = restoreSP(argv[1]);
//    //cout << final << endl;
//    final = final*255;
//    final.convertTo(final,CV_8UC3);
//    cvtColor(final,final,CV_RGB2BGR);
//    imwrite("rest.png",final);
    //hist = getHistSP(argv[1]);


    return 0;

}

