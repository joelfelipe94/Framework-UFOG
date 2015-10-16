#include "rangefiltprior.h"



Mat RangeFiltPrior::computePrior(Mat image, int patchsize,Vec3f veil)
{
    vector<Mat> canais;
    split(image,canais);
    double *arrayImagem=new double [image.rows*image.cols];
    double **arraySigma=new double* [3];
    arraySigma[0] = new double [image.rows*image.cols];
    arraySigma[1] = new double [image.rows*image.cols];
    arraySigma[2] = new double [image.rows*image.cols];
    int linhas=image.rows,colunas=image.cols;

    for(int k=0;k<3;k++){
    canais[k].convertTo(image, CV_64F);
    //unsigned char arraySigma[imagemOriginal.rows+imagemOriginal.cols];
    image=image/veil[k];
    if(image.isContinuous()){
        arrayImagem=(double *) image.data;

    }


    #pragma omp parallel for
    for(int i=patchsize/2;i<linhas-patchsize/2;i++){
        for(int j=patchsize/2;j<colunas-patchsize/2;j++){
            double minimo=1,maximo=0;

            for(int ji=-patchsize/2;ji<=patchsize/2;ji++)
                for(int jj=-patchsize/2;jj<=patchsize/2;jj++){
                    minimo=min(arrayImagem[(i+ji)*colunas+(j+jj)],minimo);
                    maximo=max(arrayImagem[(i+ji)*colunas+(j+jj)],maximo);
                }
            arraySigma[k][(i)*colunas+(j)]= (maximo-minimo);
        }
    }

    //Mat resultado=Mat(linhas,colunas,CV_8U,arraySigma);
    }
    #pragma omp parallel for
    for(int indice=0;indice<linhas*colunas;indice++){
            if(arraySigma[0][indice]<arraySigma[1][indice])
                arraySigma[0][indice]=arraySigma[1][indice];
            if(arraySigma[0][indice]<arraySigma[2][indice])
                arraySigma[0][indice]=arraySigma[2][indice];
    }
    Mat resultado=Mat(linhas,colunas,CV_64F,arraySigma[0]);
    return resultado;
}

Mat RangeFiltPrior::computeTransmission(Mat image, int patchsize, Vec3f veil)
{
    return computePrior(image,patchsize,veil);
}
