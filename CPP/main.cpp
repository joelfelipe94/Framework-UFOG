#include "common.h"
#include "l0smoothing.h"
#include "signalprior.h"
#include "darkchannelprior.h"
#include "colorconstancy.h"
#include "standartDeviation.h"
#include "bilateralfilter.h"
#include "restoration.h"
#include "dcpgb.h"
#include "colorlines.h"
#include "chromaticprior.h"
#include "redchannelprior.h"

Mat getHistSP(char * imageName){


// For now we are using just an image after u can put the folder to process.

Mat image;

cout << imageName << endl;

image = imread(imageName, CV_LOAD_IMAGE_COLOR);
image.convertTo(image, CV_32FC3);
image = image/255;
image = fitSize( image,512);
//cout << image << endl;


SignalPrior sp;

//image = filter.doFilter(image);
//Mat imageR,imageG,imageB;
ColorConstancy cp(2);


const double sigmaColor = 100.0;
    const double sigmaSpace = 10.0;
    cvtColor(image, image, CV_BGR2Lab);
    vector<Mat> channels;
split(image,channels);
cv_extend::bilateralFilter(channels[0], channels[0], sigmaColor, sigmaSpace);

merge(channels,image);
cvtColor(image, image, CV_Lab2RGB);
Vec3f veil = cp.getLightSource(image);
cout << veil << endl;


Mat transmission = sp.computePrior(image,image,15,veil);


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

Mat getHistCP(char * imageName){


// For now we are using just an image after u can put the folder to process.

Mat image,image_out;

cout << imageName << endl;

image = imread(imageName, CV_LOAD_IMAGE_COLOR);
image.convertTo(image, CV_32FC3);
image = image/255;
image = fitSize( image,1024);
//cout << image << endl;


SignalPrior sp;

//image = filter.doFilter(image);
//Mat imageR,imageG,imageB;
ColorConstancy cp(2);



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


Mat transmission = sp.computePrior(image_out,image,15,veil);


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


Mat getHistDCPGB(char * imageName){


// For now we are using just an image after u can put the folder to process.

Mat image;

cout << imageName << endl;

image = imread(imageName, CV_LOAD_IMAGE_COLOR);
image.convertTo(image, CV_64FC3);
image = image/255;

//cout << image << endl;
image = fitSize( image,1024);

DCPGB sp;


//image = filter.doFilter(image);
//Mat imageR,imageG,imageB;

//
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


Mat getHistDCP(char * imageName){


// For now we are using just an image after u can put the folder to process.

Mat image;

cout << imageName << endl;

image = imread(imageName, CV_LOAD_IMAGE_COLOR);
image.convertTo(image, CV_64FC3);
image = image/255;

//cout << image << endl;
image = fitSize( image,1024);

DarkChannelPrior sp;


//image = filter.doFilter(image);
//Mat imageR,imageG,imageB;

//
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


Mat getTransCP(char * imageName){


// For now we are using just an image after u can put the folder to process.

Mat image;

cout << imageName << endl;

image = imread(imageName, CV_LOAD_IMAGE_COLOR);
image.convertTo(image, CV_32FC3);
image = image/255;
image = fitSize( image,1024);
//cout << image << endl;


ChromaticPrior sp;

//image = filter.doFilter(image);
//Mat imageR,imageG,imageB;
ColorConstancy cp(2);


const double sigmaColor = 100.0;
    const double sigmaSpace = 10.0;
    cvtColor(image, image, CV_BGR2Lab);
    vector<Mat> channels;
split(image,channels);
cv_extend::bilateralFilter(channels[0], channels[0], sigmaColor, sigmaSpace);

merge(channels,image);
cvtColor(image, image, CV_Lab2RGB);
Vec3f veil = cp.getLightSource(image);
cout << veil << endl;


Mat transmission = sp.computePrior(image,15,veil);
transmission = transmission*255;

 //cout << transmission << endl;

transmission.convertTo(transmission,CV_8U);

return transmission;

//cout << hist << endl;




}



Mat restoreSP(Mat image)
{

        Mat image_out;
        image = fitSize( image,512);
        //Rect ROI(7,7,image.cols-14,image.rows-14);
        //copyMakeBorder(image, image, 7, 7, 7, 7, BORDER_REPLICATE);
        //cout << image << endl;



        SignalPrior sp;

        //image = filter.doFilter(image);
        //Mat imageR,imageG,imageB;
        ColorConstancy cp(2);
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

        Mat transmission = sp.computePrior(image_out,image,7,veil);
        writeToColorMap(transmission ,"colorMap.png");

      //  ColorConstancy cp_true(4);

        //Mat tImage = transmission*255;
        //tImage.convertTo(tImage,CV_8U);

        //cp_true.setTrans(tImage);

        //cout << transmission << endl;
        //image = image *255;
        //image.convertTo(image, CV_8UC3);

        //veil = cp_true.getLightSource(image);

        //cout << veil << endl;

        //transmission = sp.computePrior(image_out,image,15,veil);




//        writeImage8U(transmission,strcat(imageName,"Transmission.png"));
          Restoration restore;
//        transmission = restore.refineTransmission(image,transmission);


//        transmission.convertTo(transmission,CV_32F);
//        transmission = transmission/255;
//        writeImage8U(transmission,strcat(imageName,"TransmissionFinal.png"));

        cout << " the end " << endl;
        //Mat tImage = transmission*255;
        //tImage.convertTo(tImage,CV_8U);
        //writeImage8U(transmission,strcat(imageName,"TransmissionFinal.png"));
        //imwrite("trans.png",tImage);

         //cout << transmission << endl;

        //
        //cout << image << endl;
        cout<<"aqui"<<endl;

        image=restore.restoreImageVeil(image, transmission,  veil);
        return image;



}
Mat restoreSPfile(char * imageName)
{
    // For now we are using just an image after u can put the folder to process.

        Mat image, image_out;

        cout << imageName << endl;

        image = imread(imageName, CV_LOAD_IMAGE_COLOR);
        image.convertTo(image, CV_32FC3);
        image = image/255;


        return restoreSP(image);
        //cout << hist << endl;

}


int restoreVideoSP(char * imageName)
{
    cv::VideoCapture cap(imageName);
    if(!cap.isOpened()) {
        std::cout << "Unable to open the camera\n";
        std::exit(-1);
    }

    // Get the width/height and the FPS of the vide
    int width = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    double FPS = cap.get(CV_CAP_PROP_FPS);

    // Open a video file for writing (the MP4V codec works on OS X and Windows)
    cv::VideoWriter out("output.mov", CV_FOURCC('m','p', '4', 'v'), FPS, cv::Size(1024, 576));
    if(!out.isOpened()) {
        std::cout <<"Error! Unable to open video file for output." << std::endl;
        std::exit(-1);
    }
    cout << " fps " << FPS << endl;
    cv::Mat image;

    int frames =130;
    int count =0;
    while(count < frames) {
        cap >> image;
        if(image.empty()) {
            std::cout << "Can't read frames from your camera\n";
            break;
        }

        // Copy the middle panel from the image and convert it to gray
        //Mat top = image(cv::Rect(0, image.rows/2, image.cols, image.rows/2));
        //cout << top;

        image.convertTo(image, CV_32FC3);
        image = image/255;

        Mat rest = restoreSP(image);


        rest = rest*255;
            rest.convertTo(rest,CV_8UC3);
            cvtColor(rest,rest,CV_RGB2BGR);
        imwrite("rest.png",rest);

        //cout <<" Size out " << cv::Size(width, height/2) << endl;

        // Save frame to video
        out << rest;



        // Stop the camera if the user presses the "ESC" key
        count++;
    }
    return 0;

}


Mat restoreDCP(char * imageName){


// For now we are using just an image after u can put the folder to process.

    Mat image, image_out;

    cout << imageName << endl;

    image = imread(imageName, CV_LOAD_IMAGE_COLOR);
    image = fitSize( image,600);
    image.convertTo(image, CV_32FC3);
    image = image/255;
    //image = fitSize( image,500);
    //cout << image << endl;


    DarkChannelPrior sp;

    //image = filter.doFilter(image);
    //Mat imageR,imageG,imageB;
    ColorConstancy cp(3);


    const double sigmaColor = 100.0;
        const double sigmaSpace = 10.0;
        cvtColor(image, image_out, CV_BGR2Lab);
        vector<Mat> channels;
    split(image_out,channels);
    cv_extend::bilateralFilter(channels[0], channels[0], sigmaColor, sigmaSpace);

    merge(channels,image_out);
    cvtColor(image_out, image_out, CV_Lab2RGB);
     cvtColor(image, image, CV_BGR2RGB);


    cp.setDCP(sp.computePrior(image,15));
    Vec3f veil = cp.getLightSource(image);
    cout << veil << endl;
    //cv::resize(image,image,Size(),0.5,0.5);
    Mat transmission = sp.computeTransmission(image,7,veil);



    Restoration restore;
    //writeImage8U(transmission,strcat(imageName,"TransmissionBefore.png"));
    //cout << transmission << endl;
    transmission = restore.refineTransmission(image,transmission);
    transmission.convertTo(transmission,CV_32F);
    transmission = transmission/255;
    cout << " the end " << endl;
    //writeToColorMap(transmission ,"colorMap.png");
//    writeImage8U(transmission,strcat(imageName,"TransmissionDCP.png"));

    //cout << transmission << endl;
    //transmission = transmission;
    //transmission = transmission*255;

     //cout << transmission << endl;

    //transmission.convertTo(transmission,CV_8U);


    //cout << image << endl;
    cvtColor(image, image, CV_BGR2RGB);
    return restore.restoreImageClassic(image/255, transmission,  veil);


    //cout << hist << endl;



}

Mat restoreRed(char * imageName){


// For now we are using just an image after u can put the folder to process.

    Mat image, image_out;

    cout << imageName << endl;

    image = imread(imageName, CV_LOAD_IMAGE_COLOR);
    image = fitSize( image,600);
    image.convertTo(image, CV_32FC3);
    image = image/255;
    //image = fitSize( image,500);
    //cout << image << endl;


    RedChannelPrior sp;

    //image = filter.doFilter(image);
    //Mat imageR,imageG,imageB;
    ColorConstancy cp(5);


    const double sigmaColor = 100.0;
    const double sigmaSpace = 10.0;
    cvtColor(image, image_out, CV_BGR2Lab);
    vector<Mat> channels;
    split(image_out,channels);
    cv_extend::bilateralFilter(channels[0], channels[0], sigmaColor, sigmaSpace);

    merge(channels,image_out);
    cvtColor(image_out, image_out, CV_Lab2RGB);
    cvtColor(image, image, CV_BGR2RGB);


    cp.setDCP(sp.computePrior(image,15));
    Vec3f veil = cp.getLightSource(image);
    cout << veil << endl;
    //cv::resize(image,image,Size(),0.5,0.5);
    Mat transmission = sp.computeTransmission(image,7,veil);



    Restoration restore;
    //writeImage8U(transmission,strcat(imageName,"TransmissionBefore.png"));
    //cout << transmission << endl;
    transmission = restore.refineTransmission(image,transmission);
    transmission.convertTo(transmission,CV_32F);
    transmission = transmission/255;
    cout << " the end " << endl;
    //writeToColorMap(transmission ,"colorMap.png");
//    writeImage8U(transmission,strcat(imageName,"TransmissionDCP.png"));

    //cout << transmission << endl;
    //transmission = transmission;
    //transmission = transmission*255;

     //cout << transmission << endl;

    //transmission.convertTo(transmission,CV_8U);


    //cout << image << endl;
    cvtColor(image, image, CV_BGR2RGB);
    return restore.restoreImageRedChannel(image/255, transmission,  veil);


    //cout << hist << endl;



}





int runGroundTruthTest(char * imagesPath, char * groundTruth)
{


    DIR * dirp;
    dirp = opendir(imagesPath);
    if (dirp == NULL)
            return (1);

    dirent * dp;
    dp = readdir(dirp);
    dp = readdir(dirp);
    int N =2000;
    int count = 0;

    Mat trans;

    Mat gt;


    gt = imread(groundTruth, CV_LOAD_IMAGE_GRAYSCALE);


    Mat diffVec = Mat::zeros(256,1,CV_32F);



    normalize(gt, gt, 0, 255, NORM_MINMAX, CV_8UC1);

    gt = 255 - gt;
    int i=0;

    while ((dp = readdir(dirp)) != NULL && count < N) {


        cout << dp->d_name << endl;
        cout << count << endl;
        char fullPath[256];
        sprintf(fullPath,"%s/%s",imagesPath,dp->d_name);


        trans = getTransCP(fullPath);

        normalize(trans, trans, 0, 255, NORM_MINMAX, CV_8UC1);

        Mat diff = trans - gt;
        diffVec.at<float>(i) = mean(diff)[0];
        i++;

   }


    writeMatToFile(diffVec,"hist.txt");






    return 0;




}

int runDatasetHist(char * datasetPath)
{

    DIR * dirp;
    dirp = opendir(datasetPath);
    if (dirp == NULL)
            return (1);

    dirent * dp;

    int N =2000;
    int count = 0;
    Mat averageHist = Mat::zeros(256,1,CV_32F);
    Mat hist;
    while ((dp = readdir(dirp)) != NULL && count < N) {
    if(string(dp->d_name)==".." ||string(dp->d_name)== ".")
        continue;

        cout << dp->d_name << endl;
        cout << count << endl;
        char fullPath[256];
        sprintf(fullPath,"%s/%s",datasetPath,dp->d_name);
        hist = getHistCP(fullPath);
        cout << averageHist.rows << " " << hist.rows << endl;
        cout << hist.type() << endl;
        averageHist =  hist + averageHist;
        cout << " SUMOU " << endl;
        count++;

   }
    averageHist = averageHist/N;
    cout << averageHist << endl;
    writeMatToFile(averageHist,"hist.txt");
    drawHist(averageHist);
    return 0;
}


int runDatasetRestore(char * datasetPath)
{

    DIR * dirp;
    dirp = opendir(datasetPath);
    if (dirp == NULL)
            return (1);

    dirent * dp;

    int N =2000;
    int count = 0;
    Mat resp;
    while ((dp = readdir(dirp)) != NULL && count < N) {
    if(string(dp->d_name)==".." ||string(dp->d_name)== ".")
        continue;

        cout << dp->d_name << endl;
        cout << count << endl;
        char fullPath[256];
        sprintf(fullPath,"%s/%s",datasetPath,dp->d_name);
        resp = restoreRed(fullPath);
        sprintf(fullPath,"%s/r%s",datasetPath,dp->d_name);
        cvtColor(resp,resp,CV_RGB2BGR);
        writeImage8U(resp,fullPath);
   }
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
    int output;
    //output = runDatasetRestore(argv[1]);
    //imwrite("hist.png",hist);

    //hist = getHistSTD(argv[1]);

    //ColorLines cl;
    //Vec3f ori = cl.findOrientation();


    //restoreVideoSP(argv[1]);

    Mat final;
    final = restoreSPfile(argv[1]);
//    //cout << final << endl;
    final = final*255;
    final.convertTo(final,CV_8UC3);
    cvtColor(final,final,CV_RGB2BGR);
    imwrite("rest.png",final);
    //hist = getHistSP(argv[1]);


    return 0;

}
