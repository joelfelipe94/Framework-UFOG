#include "chiangprior.h"
#include "colorconstancy.h"

ChiangPrior::ChiangPrior()
{

    Nrer[0] = 0.97;
    Nrer[1] = 0.92;
    Nrer[2] = 0.92;
}

double ChiangPrior::prior(Mat tile)
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


Mat ChiangPrior::computePrior(Mat image, int patchsize)
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

Mat ChiangPrior::computeTransmission(Mat image, int patchsize, Vec3f veil)
{
    //[y x ~] = size(I);
    //J = zeros(y, x);

    // add some space around the edges

    int sizeY = image.rows;
    int sizeX = image.cols;

    cout << sizeX << endl;
    cout << sizeY << endl;
    Mat transmission(sizeY,sizeX,CV_32FC3);


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

            priorValue = -log(priorValue);
            Vec3f priorRGB(pow(Nrer[0],priorValue),pow(Nrer[1],priorValue),pow(Nrer[2],priorValue));





            //cout << tile.rows << " " << tile.cols << endl;
            //cout << i << " " << j << endl;
            //cout << transmission.rows << " " <<  transmission.cols << endl;
            //cout << Rect(i,j, tile.rows ,tile.cols) << endl;
             // Yes, the rectangle is inverted with matrix when you access
           // Mat tileCopy = transmission(Rect(j,i, tile.cols ,tile.rows));
            transmission.at<Vec3f>(i,j) = priorRGB;


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


double ChiangPrior::findDepth(Vec3f airlight,Vec3f ambientlight)
{

    double  minDistance = 100000;

    double depth =0;

    for (int j=0; j<100000; j++){
        double value  = double(j)/1000;
        double distance = 0;
        for (int i=0;i<3;i++){
            distance += pow(ambientlight[i] - airlight[i]*pow(Nrer[i],value),2);
        }

        if (distance < minDistance){
            minDistance = distance;
            depth = value;
        }

    }


        return depth;




}

Mat ChiangPrior::generateDepthImage(Mat image, Mat trans)
{
    ColorConstancy cc(3);


    /// set transmission

    Mat topImage = image(Rect(0,0,image.cols,int(image.rows/2)));
    Mat bottonImage = image(Rect(0,int(image.rows/2),image.cols,int(image.rows/2)));

    Mat topTrans = trans(Rect(0,0,trans.cols,int(trans.rows/2)));
    Mat bottonTrans = trans(Rect(0,int(trans.rows/2),trans.cols,int(trans.rows/2)));


    cout  << "cut image  "  << endl;


    Vec3f airlight(0.83,0.83,0.83);

        cc.setDCP(topTrans);


    Vec3f topAmbient = cc.getLightSource(topImage);
    cc.setDCP(bottonTrans);


    Vec3f bottonAmbient = cc.getLightSource(bottonImage);

    Mat depthImage(image.rows,image.cols,CV_32FC3);


    cout  << " go to find depth "  << endl;

    cout << topAmbient << endl;

    cout << bottonAmbient << endl;

    double d1 = findDepth(airlight,topAmbient);


    double d2 = findDepth(airlight,bottonAmbient);



    double r = abs(d1 - d2);

    cout << " d1 " << d1 << " d2 " << d2 << " r " << r <<  endl;


    cout << " rows " << image.rows << " " << image.cols << endl;


    for (int i=0; i < image.rows;i++){

        double b =0;
        double c = image.rows;

        double  depth  = d1 + r*((i-b)/(c-b));

        //cout << depth << endl;
        // just to put on the image

        Vec3f depthRGB(pow(Nrer[0],depth),pow(Nrer[1],depth),pow(Nrer[2],depth));

        for (int j=0; j < image.cols;j++){



            depthImage.at<Vec3f>(i,j) = depthRGB;


        }


    }

    return depthImage;






}






