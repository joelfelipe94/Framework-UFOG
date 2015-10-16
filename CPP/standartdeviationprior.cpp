#include <iostream>
#include <vector>
#include "cv.h"
#include "highgui.h"
#include <omp.h>
#include "standartdeviationprior.h"


StandartDeviationPrior::StandartDeviationPrior(){

}

Mat StandartDeviationPrior::computePrior(Mat imagemOriginal,int n){

    double maximo=0;
    vector<Mat> canais;
    split(imagemOriginal,canais);
    double *arrayImagem=new double [imagemOriginal.rows*imagemOriginal.cols];
    double **arraySigma=new double* [3];
    arraySigma[0] = new double [imagemOriginal.rows*imagemOriginal.cols];
    arraySigma[1] = new double [imagemOriginal.rows*imagemOriginal.cols];
    arraySigma[2] = new double [imagemOriginal.rows*imagemOriginal.cols];
    int linhas=imagemOriginal.rows,colunas=imagemOriginal.cols;

    for(int k=0;k<3;k++){
  //  canais[k].convertTo(imagemOriginal, CV_32F);
    //unsigned char arraySigma[imagemOriginal.rows+imagemOriginal.cols];

    if(imagemOriginal.isContinuous()){
        arrayImagem=(double *) imagemOriginal.data;

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
                    temp*=2;
                    soma+=temp*temp;
                }
            //soma=sqrt(soma/(double((n*n)-1)));
            soma=sqrt(soma);
            maximo=max(maximo,soma);
            arraySigma[k][(i)*colunas+(j)]= (double) soma;
            //cout<<arraySigma[k][(i)*colunas+(j)]<<endl;
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
    resultado/=maximo;
    return resultado;

}
  Mat StandartDeviationPrior::computeTransmission(Mat imagemOriginal,int n){
      computePrior(imagemOriginal,n); //TODO fazer soft matting
  }

