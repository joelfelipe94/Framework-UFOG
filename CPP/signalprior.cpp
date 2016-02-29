#include "signalprior.h"
#include "gradients1prior.h"
#include "standartdeviationprior.h"
#include "chromaticprior.h"
#include "rangefiltprior.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "restoration.h"

double SignalPrior::prior(Mat tile,Vec3f veil)
{
}

Mat SignalPrior::computePrior(Mat imageBilateral, Mat image, int patchsize, Vec3f veil)
{


//        GradienteS1Prior grad;
        ChromaticPrior chrom;
        RangeFiltPrior range;
//        StandardDeviationPrior stand;
        Mat gradiente,desvio,rangeFilt,chromatic;
//        desvio=stand.computePrior(image,patchsize);
//        gradiente=grad.computePrior(image,patchsize);
        rangeFilt=range.computePrior(image,patchsize,veil);
        //Rect ROI(patchsize,patchsize,rangeFilt.cols-2*patchsize,rangeFilt.rows-2*patchsize);
        writeImage8U(rangeFilt,"babacaRangeFilt.png");
        //blur(rangeFilt,rangeFilt,Size(15,15));
        chromatic = chrom.computePrior(imageBilateral,patchsize,veil);
        //Rect ROe(patchsize,patchsize,chromatic.cols-2*patchsize,chromatic.rows-2*patchsize);
        writeImage8U(chromatic,"babacaChroma.png");
       // gradiente=guidedFilter(image,gradiente,60,0.01);
 //       writeImage8U(gradiente,"gradiente.png");
        Restoration test;

        Mat transRange =  test.refineTransmission(image.clone(),rangeFilt);
        imwrite("babacaRangeFiltSoft.png",transRange);

        transRange.convertTo(transRange,CV_32F);
        transRange = transRange/255;

        rangeFilt.convertTo(rangeFilt,CV_32F);
        rangeFilt = rangeFilt/255;
        writeToColorMap(rangeFilt ,"colorMaprange.png");

        Mat transChroma = test.refineTransmission(image.clone(),chromatic);
        imwrite("babacaChromaSoft.png",transChroma);

        transChroma.convertTo(transChroma,CV_32F);
        transChroma = transChroma/255;

        chromatic.convertTo(chromatic,CV_32F);
        chromatic = chromatic/255;
        writeToColorMap(chromatic ,"colorMapChroma.png");

//        Mat transChroma = chromatic;
//        Mat transRange = rangeFilt;

        //Mat rangeTrans =  abs(transRange - transChroma)/2;

        //Mat alpha = 0.7 - 0.546*rangeTrans;



//        Restoration test;

//        Mat transRange =  test.refineTransmission(image.clone(),rangeFilt);
//        transRange.convertTo(transRange,CV_32F);
//        transRange = transRange/255;
//        Mat transChroma = test.refineTransmission(image.clone(),chromatic);
//        transChroma.convertTo(transChroma,CV_32F);
//        transChroma = transChroma/255;
//        //cout << transChroma << endl;
//        Mat signal  = max(chromatic,rangeFilt);

//        Mat transFinal = test.refineTransmission(image.clone(),signal/255);
//        transFinal.convertTo(transFinal,CV_32F);
//        transFinal = transFinal/255;
//        //cout << transFinal << endl;


//        writeImage8U(transRange,"rangeFilt.png");
//        writeImage8U(desvio,"deviation.png");
//        writeImage8U(transChroma,"chromatic.png");

//        writeImage8U(transFinal,"final.png");

//        Mat DiffChroma, DiffRange;

//        DiffRange = abs(transFinal - transRange);
//        DiffChroma = abs(transFinal - transChroma);

       // writeImage8U(transChroma,"chromatic.png");

       // writeImage8U(transRange,"range.png");


//        writeImage8U(max(chromatic,desvio),"transmissao.png");

        // writeImage8U(alpha,"alpha.png");

        //return transChroma.mul(alpha) + transRange.mul(1-alpha);
        vector<Mat> channels;


        Mat maxChroma;
        Mat maxRange;
        Mat mask;



        mask = (transRange > transChroma)/255;

        //cout << mask << endl;

        //cout << mask.type() << endl;
          mask.convertTo(mask,CV_32F);

        maxRange = transRange.mul(mask);
        maxChroma = transChroma.mul(1- mask);





        Mat resp;
        channels.resize(3);
        channels[0]=Mat::zeros(transChroma.rows,transChroma.cols,CV_32F);
        channels[1]=Mat::zeros(transChroma.rows,transChroma.cols,CV_32F);
        channels[2]=Mat::zeros(transChroma.rows,transChroma.cols,CV_32F);



        channels[1]=maxRange;
        channels[0]=maxChroma;


        merge(channels,resp);
        writeImage8U(resp,"transmissaoColorSoft.png");


        return max(transChroma,transRange);
        //return chromatic;


}

Mat SignalPrior::computeTransmission(Mat imageBilateral,Mat image, int patchsize, Vec3f veil)
{
    return computePrior(imageBilateral,image,patchsize,veil);
}
