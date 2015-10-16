#include "dcpgb.h"

DCPGB::DCPGB()
{
}

double DCPGB::prior(Mat tile)
{

    double minValG;
    double minValB;

    double maxValG;
    double maxValB;
    Point minLoc;
    Point maxLoc;
    vector<Mat> channels;
    double priorValue;


    split(tile,channels);





      // cout << channels[0] << endl;

    minMaxLoc( channels[1], &minValG, &maxValG, &minLoc, &maxLoc );
    minMaxLoc( channels[2], &minValB, &maxValB, &minLoc, &maxLoc );
    //cout << maxValR << " " << maxValG << " " << maxValB << endl;

    priorValue =MIN(minValB,minValG);


    return priorValue;



}

Mat DCPGB::computePrior(Mat image, int patchsize)
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

                double priorValue = prior(tile);
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

Mat DCPGB::computeTransmission(Mat image, int patchsize, Vec3f veil)
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
            vector<Mat> tChannels;
            split(tile,tChannels);
            tChannels[0] = tChannels[0]/veil[0];
            tChannels[1] = tChannels[1]/veil[1];
            tChannels[2] = tChannels[2]/veil[2];
            merge(tChannels,tile);
            double priorValue = 1- prior(tile);
            //cout << tile.rows << " " << tile.cols << endl;
            //cout << i << " " << j << endl;
            //cout << transmission.rows << " " <<  transmission.cols << endl;
            //cout << Rect(i,j, tile.rows ,tile.cols) << endl;
             // Yes, the rectangle is inverted with matrix when you access
           // Mat tileCopy = transmission(Rect(j,i, tile.cols ,tile.rows));
            transmission.at<double>(i,j) =  priorValue;
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
