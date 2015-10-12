#include "common.h"
#include "kernels.cu"
#include <eigen3/Eigen/Sparse>


using namespace cv;
using namespace Eigen;

//#define WINSIZE 1
//const int nebsize=(WINSIZE*2+1)*(WINSIZE*2+1);
//tudo isso ja foi definido em kernels.cpp

//se aumentar o tamanho de WINSIZE em faltar memoria pra iniciar um dos kernels, tem que diminuir esse valor
#define MAX_WARPS 16

double *dev_img, *dev_vals, *h_alpha_mtrx, *img, *img_scr, *h_vals;
int *dev_row_inds, *dev_col_inds, *n_mp_dev, *h_row_inds, *h_col_inds;
double lambda;
int tlen;
int n_vals;



void init_vars(int img_w, int img_h){
    tlen=(img_w-2*WINSIZE)*(img_h-2*WINSIZE)*nebsize*nebsize;
        n_vals=0;
    int size_vec_img=img_w*img_h*sizeof(double);
    img=(double*)malloc(3*size_vec_img);
    img_scr=(double*)malloc(size_vec_img);
    cudaMalloc((void **)&dev_img, 3*size_vec_img);
    cudaMalloc((void **)&dev_row_inds, tlen*sizeof(int));
    cudaMalloc((void **)&dev_col_inds, tlen*sizeof(int));
    cudaMalloc((void **)&dev_vals, tlen*sizeof(double));
    int dist_h, dist_w;
    for(int i=0; i<img_w*img_h; i++){
        dist_h=min(2*WINSIZE,min(i%img_h,img_h-1-i%img_h));
        dist_w=min(2*WINSIZE,min(i/img_h,img_w-1-i/img_h));
        n_vals+=(1+2*WINSIZE+dist_h)*(1+2*WINSIZE+dist_w);
    }
    h_row_inds=(int*)malloc(tlen*sizeof(int));
    h_col_inds=(int*)malloc(tlen*sizeof(int));
    h_vals=(double*)malloc(tlen*sizeof(double));

    int n_dev;
    cudaGetDeviceCount(&n_dev);
    n_mp_dev=(int*)malloc(n_dev*sizeof(int));
    cudaDeviceProp dev_prop[n_dev];
    cudaSetDevice(0);
    for(int i=0; i<n_dev; i++){
            cudaGetDeviceProperties(&dev_prop[i], i);
        n_mp_dev[i]=dev_prop[i].multiProcessorCount;
    }
}

void free_vars(){
    free(img);
    free(img_scr);
    cudaFree(dev_col_inds);
    cudaFree(dev_vals);
    cudaFree(dev_row_inds);
    cudaFree(dev_img);
}

Mat solve_alpha(const Mat *img_cv, const Mat *img_scr_cv){
    int c0, c1, c2;
    int img_w=img_cv->cols;
    int img_h=img_cv->rows;

    if(img_cv->type()==CV_8UC3){
        //se a imagem e colorida inverte os canais para converter de BGR para RGB
        c0=2;
        c1=1;
        c2=0;
    } else if(img_cv->type()==CV_8UC1){
        //se e monocromatica ja converte pra colorida
        c0=0;
        c1=0;
        c2=0;
    }
#pragma omp parallel for
    for(int i=0; i<img_w; i++){
        for(int j=0;j<img_h; j++){
            img[3*(i*img_h+j)]=img_cv->at<Vec3b>(j,i)[c0]/255.0l;
            img[3*(i*img_h+j)+1]=img_cv->at<Vec3b>(j,i)[c1]/255.0l;
            img[3*(i*img_h+j)+2]=img_cv->at<Vec3b>(j,i)[c2]/255.0l;
            img_scr[(i*img_h+j)]=img_scr_cv->at<Vec3b>(j,i)[0]/255.0l;
        }
    }

    int size_vec_img=img_w*img_h*sizeof(double);
    cudaMemcpy(dev_img, img, 3*size_vec_img, cudaMemcpyHostToDevice);

    int n_warps=(img_w*img_h+31)/32;
    int n_blocos=n_mp_dev[0];
    int warps_pb=(n_warps+n_blocos-1)/n_blocos;

    while(warps_pb>MAX_WARPS){
        n_blocos+=n_mp_dev[0];
        warps_pb=(n_warps+n_blocos-1)/n_blocos;;
    }
        double epsilon=0.000001;
    loop<<<n_blocos,32*warps_pb>>>(dev_img, dev_row_inds, dev_col_inds, dev_vals, img_w, img_h,epsilon);

    cudaMemcpy(h_row_inds, dev_row_inds, tlen*sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(h_col_inds, dev_col_inds, tlen*sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(h_vals, dev_vals, tlen*sizeof(double), cudaMemcpyDeviceToHost);

    lambda=0.0001;
    VectorXd denom(img_w*img_h);
    #pragma omp parallel for
    for(int i=0; i<img_w*img_h; i++){
        denom(i)=img_scr[i]*lambda;
    }

    SparseMatrix<double> mat(img_w*img_h,img_w*img_h);

    std::vector<Triplet<double> > tripletList;
    tripletList.resize(tlen+img_w*img_h);
    #pragma omp parallel for
    for(int i=0; i<tlen; i++){
        tripletList[i]=Triplet<double>(h_row_inds[i],h_col_inds[i],h_vals[i]);
    }
    #pragma omp parallel for
    for(int i=0; i<img_w*img_h; i++){
        tripletList[tlen+i]=Triplet<double>(i,i,lambda);
    }

    printf("threads eigen %d\n", nbThreads());

    mat.setFromTriplets(tripletList.begin(), tripletList.end());
    printf("acabou\n");
    SimplicialCholesky<SparseMatrix<double> > chol(mat);
    printf("acabou2\n");
    VectorXd alpha = chol.solve(denom);
    h_alpha_mtrx=alpha.data();

    double alpha_min, alpha_max, img_min, img_max;
    alpha_min=h_alpha_mtrx[0];
    alpha_max=h_alpha_mtrx[0];
    img_min=img_scr[0];
    img_max=img_scr[0];
    #pragma omp parallel for
    for(int i=1; i<img_w*img_h; i++){
        if(h_alpha_mtrx[i]>alpha_max){
            alpha_max=h_alpha_mtrx[i];
        } else if(h_alpha_mtrx[i]<alpha_min){
            alpha_min=h_alpha_mtrx[i];
        }
        if(img_scr[i]>img_max){
            img_max=img_scr[i];
        } else if(img_scr[i]<img_min){
            img_min=img_scr[i];
        }
    }

    alpha_max-=alpha_min;
    #pragma omp parallel for
    for(int i=0; i<img_w*img_h; i++){
        h_alpha_mtrx[i]-=alpha_min;
        h_alpha_mtrx[i]=alpha[i]/alpha_max;
        h_alpha_mtrx[i]=alpha[i]*(img_max-img_min)+img_min;
    }

    Mat alpha_img(img_h, img_w, CV_8UC1);
    #pragma omp parallel for
    for(int i=0; i<img_w; i++){
        for(int j=0;j<img_h; j++){
            alpha_img.at<uchar>(j,i)=h_alpha_mtrx[i*img_h+j]*255;
        }
    }
    return alpha_img;
}

