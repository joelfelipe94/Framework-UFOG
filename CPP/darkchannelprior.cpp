#include "darkchannelprior.h"

DarkChannelPrior::DarkChannelPrior()
{
}

double DarkChannelPrior::dcp(Mat tile)
{
    double minValR;
    double minValG;
    double minValB;
    double maxValR;
    double maxValG;
    double maxValB;
    Point minLoc;
    Point maxLoc;
    vector<Mat> channels;
    double priorValue;


    split(tile,channels);





      // cout << channels[0] << endl;


    minMaxLoc( channels[0], &minValR, &maxValR, &minLoc, &maxLoc );
    minMaxLoc( channels[1], &minValG, &maxValG, &minLoc, &maxLoc );
    minMaxLoc( channels[2], &minValB, &maxValB, &minLoc, &maxLoc );
    //cout << maxValR << " " << maxValG << " " << maxValB << endl;

    priorValue =MIN(MIN(minValR,minValG),minValB);


    return priorValue;



}

Mat DarkChannelPrior::computePrior(Mat image, int patchsize)
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

                double priorValue = dcp(tile);
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

