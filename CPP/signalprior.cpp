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
        //blur(rangeFilt,rangeFilt,Size(15,15));
        chromatic = chrom.computePrior(imageBilateral,patchsize,veil);
       // gradiente=guidedFilter(image,gradiente,60,0.01);
 //       writeImage8U(gradiente,"gradiente.png");
        Restoration test;

        Mat transRange =  test.refineTransmission(image.clone(),rangeFilt);
        transRange.convertTo(transRange,CV_32F);
        transRange = transRange/255;
        Mat transChroma = test.refineTransmission(image.clone(),chromatic);
        transChroma.convertTo(transChroma,CV_32F);
        transChroma = transChroma/255;

        //Mat transChroma = chromatic;
        //Mat transRange = rangeFilt;

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
////        writeImage8U(desvio,"deviation.png");
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


        return max(transChroma,transRange);
        //return chromatic;

}

Mat SignalPrior::computeTransmission(Mat imageBilateral,Mat image, int patchsize, Vec3f veil)
{
    return computePrior(imageBilateral,image,patchsize,veil);
}
