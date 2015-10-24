#include "signalprior.h"
#include "gradients1prior.h"
#include "standartdeviationprior.h"
#include "chromaticprior.h"
#include "rangefiltprior.h"
#include "opencv2/imgproc/imgproc.hpp"

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
        writeImage8U(rangeFilt,"rangeFilt.png");
//        writeImage8U(desvio,"deviation.png");
        writeImage8U(chromatic,"chromatic.png");
//        writeImage8U(max(chromatic,desvio),"transmissao.png");


        return max(chromatic,rangeFilt);
        //return chromatic;

}

Mat SignalPrior::computeTransmission(Mat imageBilateral,Mat image, int patchsize, Vec3f veil)
{
    return computePrior(imageBilateral,image,patchsize,veil);
}
