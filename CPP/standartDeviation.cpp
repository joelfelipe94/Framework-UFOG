#include <iostream>
#include <vector>
#include "cv.h"
#include "highgui.h"
#include <omp.h>
#include "standartDeviation.h"


StandardDeviation::StandardDeviation(){

}

Mat StandardDeviation::computePrior(Mat imagemOriginal,int n){

    vector<Mat> canais;
    split(imagemOriginal,canais);
    uchar *arrayImagem=new uchar [imagemOriginal.rows*imagemOriginal.cols];
    uchar **arraySigma=new uchar* [3];
    arraySigma[0] = new uchar [imagemOriginal.rows*imagemOriginal.cols];
    arraySigma[1] = new uchar [imagemOriginal.rows*imagemOriginal.cols];
    arraySigma[2] = new uchar [imagemOriginal.rows*imagemOriginal.cols];
    int linhas=imagemOriginal.rows,colunas=imagemOriginal.cols;

    for(int k=0;k<3;k++){
    canais[k].convertTo(imagemOriginal, CV_8U);
    //unsigned char arraySigma[imagemOriginal.rows+imagemOriginal.cols];

    if(imagemOriginal.isContinuous()){
        arrayImagem=(unsigned char *) imagemOriginal.data;

    }


    #pragma omp parallel for
    for(int i=n/2;i<linhas-n/2;i++){
        for(int j=n/2;j<colunas-n/2;j++){
            double media=0.0,soma=0.0,temp;
            for(int ji=-n/2;ji<=n/2;ji++)
                for(int jj=-n/2;jj<=n/2;jj++)
                    media+=arrayImagem[(i+ji)*colunas+(j+jj)];
            media/=n*n;
            for(int ji=-n;ji<=n;ji++)
                for(int jj=-n;jj<=n;jj++){
                    temp=arrayImagem[(i+ji)*colunas+(j+jj)]-media;
                    soma+=temp*temp;
                }
            soma=sqrt(soma/(double((n*n)-1)));
            arraySigma[k][(i)*colunas+(j)]= (unsigned char) soma;
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
    Mat resultado=Mat(linhas,colunas,CV_8U,arraySigma[0]);
    return resultado;

}
