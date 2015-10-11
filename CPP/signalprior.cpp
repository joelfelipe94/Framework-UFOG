#include "signalprior.h"

SignalPrior::SignalPrior()
{
}

double SignalPrior::priorBinf(Mat tile,Vec3f veil)
{
    double minVal;
    double maxValR;
    double maxValG;
    double maxValB;
    Point minLoc;
    Point maxLoc;
    vector<Mat> channels;
    double priorValue;


    split(tile,channels);




    channels[0] = abs((veil[0] - channels[0])/veil[0]);
    channels[1] = abs((veil[1] - channels[1])/veil[1]);
    channels[2] = abs((veil[2] - channels[2])/veil[2]);

      // cout << channels[0] << endl;


    minMaxLoc( channels[0], &minVal, &maxValR, &minLoc, &maxLoc );
    minMaxLoc( channels[1], &minVal, &maxValG, &minLoc, &maxLoc );
    minMaxLoc( channels[2], &minVal, &maxValB, &minLoc, &maxLoc );
    //cout << maxValR << " " << maxValG << " " << maxValB << endl;

    priorValue =double((MAX(maxValR,maxValG))/MAX(abs(1-MIN(MIN(veil[0],veil[1]),veil[2])),abs(MAX(MAX(veil[0],veil[1]),veil[2]))));



    return priorValue;



}

double SignalPrior::prior(Mat tile,Vec3f veil)
{
    double minVal;
    double maxValR;
    double maxValG;
    double maxValB;
    Point minLoc;
    Point maxLoc;
    vector<Mat> channels;
    double priorValue;


    split(tile,channels);




    channels[0] = abs(veil[0] - channels[0]);
    channels[1] = abs(veil[1] - channels[1]);
    channels[2] = abs(veil[2] - channels[2]);

      // cout << channels[0] << endl;


    minMaxLoc( channels[0], &minVal, &maxValR, &minLoc, &maxLoc );
    minMaxLoc( channels[1], &minVal, &maxValG, &minLoc, &maxLoc );
    minMaxLoc( channels[2], &minVal, &maxValB, &minLoc, &maxLoc );
    //cout << maxValR << " " << maxValG << " " << maxValB << endl;

    priorValue =double((MAX(maxValR,maxValG))/MAX(abs(1-MIN(MIN(veil[0],veil[1]),veil[2])),abs(MAX(MAX(veil[0],veil[1]),veil[2]))));



    return priorValue;



}


Mat SignalPrior::computePrior(Mat image, int patchsize , Vec3f veil)
{


        //[y x ~] = size(I);
        //J = zeros(y, x);

        // add some space around the edges

        int sizeY = image.rows;
        int sizeX = image.cols;

        cout << sizeX << endl;
        cout << sizeY << endl;
        Mat transmission(sizeY,sizeX,CV_64F);


        cout << image(Range(sizeY-1,sizeY-1),Range(sizeX-1,sizeX-1)) << endl;
        for(int i=0; i < sizeY;i++){
            for(int j=0;j < sizeX;j++){
                //cout << j + patchsize << " " << image.cols << endl;

                Mat tile = image(Range(i, min(i + patchsize, image.rows)), Range(j, min(j + patchsize, image.cols))).clone();

                //Mat tileCopy =transmission(Range(i, min(i + patchsize, transmission.rows)), Range(j, min(j + patchsize, transmission.cols)));

                //cout << tile.channels() << endl;
                //cout << i << " " << j << endl;

                double priorValue = prior(tile,veil);
                //cout << tile.rows << " " << tile.cols << endl;
                //cout << i << " " << j << endl;
                //cout << transmission.rows << " " <<  transmission.cols << endl;
                //cout << Rect(i,j, tile.rows ,tile.cols) << endl;
                 // Yes, the rectangle is inverted with matrix when you access
               // Mat tileCopy = transmission(Rect(j,i, tile.cols ,tile.rows));
                transmission.at<double>(i,j) = priorValue;
                //cout <<  tileCopy.rows << " " << tileCopy.cols << endl;
                //tileCopy =  Mat( tile.rows,tile.cols, CV_64F, cvScalar(priorValue));
                //tileCopy.copyTo(transmission(Rect(j,i, tile.cols ,tile.rows)));
                //cout << "prior" << endl;
                //cout << priorValue << endl;
                //cout << transmission.at<double>(i,j) << endl;
                //cout << priorValue << " ";
                //tileCopy =

            }
            //cout << i  << endl;
        }


    return transmission;
}
Mat SignalPrior::signalPrior(Mat image, int patchsize,Vec3f veil)
{
    StandardDeviation std;

    Mat charimage = image*255;
    charimage.convertTo(charimage,CV_8U);

    Mat deviation = std.computePrior(charimage,patchsize);
    /// Change this prior to a general one

    Mat chromatic = computePrior(image,patchsize,veil);


    //cout << deviation << endl;

    deviation.convertTo(deviation,CV_64F);
    deviation = deviation/255;

    return max(deviation,chromatic);


}

Mat SignalPrior::computeTransmission(Mat image, int patchsize , Vec3f veil){

    /// For the case of signal prior , this is exactly the same.
    return computePrior( image,  patchsize ,  veil);

}

/*
SignalPrior::obtainTransmission(Mat image, int scale){


    // image file
    image = 'Images/Water/merg.png';
    scale = 0.4;

    // used when generating the prior: width of the patch that is examined
    // to determine the dark channel value for a point
    patch_size = 15;

    // denoted by lowercase omega in He, Sun, Tang paper
    // fraction of haze to remove
    // leaving some (removal_amount < 1.0) preserves 'aerial perspective'
    removal_amount = 0.95;%0.95;

    im = imresize(imread(image), scale);
    addpath(genpath('RDCP'));
    imshow(im);
    im_rest = rdcp(im,removal_amount,patch_size);
    %rmpath RDCP;
    fprintf("Restarou\n");
    %fprintf(im_rest);
    im_rest
    figure;
    imshow(im_rest);
    title('restaurada');


}
*/
