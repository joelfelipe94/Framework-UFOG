#include "softmatting.h"
using namespace cv;
using namespace Eigen;

#define WINSIZE 1
const int nebsize=(WINSIZE*2+1)*(WINSIZE*2+1);

//se aumentar o tamanho de WINSIZE em faltar memoria pra iniciar um dos kernels, tem que diminuir esse valor
#define MAX_WARPS 16

double *h_alpha_mtrx, *img, *img_scr, *h_vals;
int *h_row_inds, *h_col_inds;
double lambda;
int tlen;
int n_vals;


void loop(double *img, int *row_inds, int *col_inds, double *vals, int w, int h, double epsilon){
    //int thread_num=blockIdx.x*blockDim.x+threadIdx.x;
    //dimensoes internas, sem contar as bordas
    int int_w=(w-2*WINSIZE);
    int int_h=(h-2*WINSIZE);
    int int_r;
    int int_c;
    double winI[3*nebsize];
    int ind;
    double winI_aux[3*nebsize];
    double pre_win_var[9];
    double win_var[9];
    double win_mu[3];
    double detwin;
    int win_inds[nebsize];
    double tvals[nebsize*nebsize];
    int k;
    //printf("entrou aqui %d %d\n", int_w, int_h);
    for(int_c=0; int_c<int_w; int_c++){
        for(int_r=0; int_r<int_h; int_r++){
            //printf("entrou\n");
            ind=(int_c+WINSIZE)*h+(int_r+WINSIZE);
            k=0;
            for(int i=-WINSIZE; i<=WINSIZE; i++){
                for(int j=-WINSIZE; j<=WINSIZE; j++){
                    win_inds[k]=ind+i*h+j;
                    k++;
                }
            }

            k=0;
            for(int i=-WINSIZE; i<=WINSIZE; i++){
                memcpy(&winI[3*k*(WINSIZE*2+1)],&img[3*(ind+i*h-WINSIZE)], 3*(WINSIZE*2+1)*sizeof(double));
                k++;
            }

            win_mu[0]=0;
            win_mu[1]=0;
            win_mu[2]=0;
            for(int i=0; i<nebsize; i++){
                win_mu[0]+=winI[3*i];
                win_mu[1]+=winI[3*i+1];
                win_mu[2]+=winI[3*i+2];
            }
            win_mu[0]=win_mu[0]/nebsize;
            win_mu[1]=win_mu[1]/nebsize;
            win_mu[2]=win_mu[2]/nebsize;
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    pre_win_var[3*i+j]=0;
                    for(int n=0; n<nebsize; n++){
                        pre_win_var[3*i+j]+=winI[3*n+i]*winI[3*n+j];
                    }
                    pre_win_var[3*i+j]=pre_win_var[3*i+j]/nebsize;
                    pre_win_var[3*i+j]+=(i==j)*epsilon/nebsize-win_mu[j]*win_mu[i];
                }
            }

            //inversa
            detwin=pre_win_var[0]*pre_win_var[4]*pre_win_var[8]+pre_win_var[2]*pre_win_var[3]*pre_win_var[7]+pre_win_var[1]*pre_win_var[5]*pre_win_var[6];
            detwin-=pre_win_var[6]*pre_win_var[4]*pre_win_var[2]+pre_win_var[3]*pre_win_var[1]*pre_win_var[8]+pre_win_var[7]*pre_win_var[5]*pre_win_var[0];

            win_var[0]=(pre_win_var[4]*pre_win_var[8]-pre_win_var[5]*pre_win_var[7])/detwin;
            win_var[3]=-(pre_win_var[3]*pre_win_var[8]-pre_win_var[5]*pre_win_var[6])/detwin;
            win_var[6]=(pre_win_var[3]*pre_win_var[7]-pre_win_var[4]*pre_win_var[6])/detwin;
            win_var[1]=-(pre_win_var[1]*pre_win_var[8]-pre_win_var[2]*pre_win_var[7])/detwin;
            win_var[4]=(pre_win_var[0]*pre_win_var[8]-pre_win_var[2]*pre_win_var[6])/detwin;
            win_var[7]=-(pre_win_var[0]*pre_win_var[7]-pre_win_var[1]*pre_win_var[6])/detwin;
            win_var[2]=(pre_win_var[1]*pre_win_var[5]-pre_win_var[2]*pre_win_var[4])/detwin;
            win_var[5]=-(pre_win_var[0]*pre_win_var[5]-pre_win_var[2]*pre_win_var[3])/detwin;
            win_var[8]=(pre_win_var[0]*pre_win_var[4]-pre_win_var[1]*pre_win_var[3])/detwin;
            //fim da inversa


            for(int i=0; i<nebsize; i++){
                    winI[3*i+0]-=win_mu[0];
                    winI[3*i+1]-=win_mu[1];
                    winI[3*i+2]-=win_mu[2];
            }

            for(int i=0; i<nebsize; i++){
                    for(int j=0; j<3; j++){
                        winI_aux[3*i+j]=0;
                        for(int n=0; n<3; n++){
                            winI_aux[3*i+j]+=winI[3*i+n]*win_var[3*n+j];
                        }
                    }
            }

            for(int i=0; i<nebsize; i++){
                for(int j=0; j<nebsize; j++){
                    tvals[i*nebsize+j]=0;
                    for(int n=0; n<3; n++){
                        tvals[i*nebsize+j]+=winI_aux[3*i+n]*winI[3*j+n];
                    }
                    tvals[i*nebsize+j]++;
                    tvals[i*nebsize+j]=(i==j)-tvals[i*nebsize+j]/nebsize;
                }
            }

            k=0;
            int base=(int_c*int_h+int_r)*nebsize*nebsize;
            for(int i=0; i<nebsize; i++){
                for(int j=0; j<nebsize; j++){
                    row_inds[base+k]=win_inds[j];
                    col_inds[base+k]=win_inds[i];
                    k++;
                }
            }
            memcpy(&vals[base],tvals, nebsize*nebsize*sizeof(double));
            /*for(int i=base; i<nebsize*nebsize; i++){
                printf("%d %d %f\n", h_row_inds[i], h_col_inds[i], h_vals[i]);
            }*/
        }
    }
}

void init_vars(int img_w, int img_h){
    tlen=(img_w-2*WINSIZE)*(img_h-2*WINSIZE)*nebsize*nebsize;
        n_vals=0;
    int size_vec_img=img_w*img_h*sizeof(double);
    img=(double*)malloc(3*size_vec_img);
    img_scr=(double*)malloc(size_vec_img);
    int dist_h, dist_w;
    for(int i=0; i<img_w*img_h; i++){
        dist_h=min(2*WINSIZE,min(i%img_h,img_h-1-i%img_h));
        dist_w=min(2*WINSIZE,min(i/img_h,img_w-1-i/img_h));
        n_vals+=(1+2*WINSIZE+dist_h)*(1+2*WINSIZE+dist_w);
    }
    h_row_inds=(int*)malloc(tlen*sizeof(int));
    h_col_inds=(int*)malloc(tlen*sizeof(int));
    h_vals=(double*)malloc(tlen*sizeof(double));
}

void free_vars(){
    free(h_row_inds);
    free(h_col_inds);
    free(h_vals);
    free(img);
    free(img_scr);
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

    int ind=0;
    for(int i=0; i<img_w; i++){
        for(int j=0;j<img_h; j++){
            img[3*ind]=img_cv->at<Vec3b>(j,i)[c0]/255.0l;
            img[3*ind+1]=img_cv->at<Vec3b>(j,i)[c1]/255.0l;
            img[3*ind+2]=img_cv->at<Vec3b>(j,i)[c2]/255.0l;
            img_scr[ind]=img_scr_cv->at<Vec3b>(j,i)[0]/255.0l;
            ind++;
        }
    }

    /*int size_vec_img=img_w*img_h*sizeof(double);
    cudaMemcpy(dev_img, img, 3*size_vec_img, cudaMemcpyHostToDevice);

    int n_warps=((img_w-2*WINSIZE)*(img_h-2*WINSIZE)+31)/32;
    int n_blocos=n_mp_dev[0];
    int warps_pb=(n_warps+n_blocos-1)/n_blocos;

    while(warps_pb>MAX_WARPS){
        n_blocos+=n_mp_dev[0];
        warps_pb=(n_warps+n_blocos-1)/n_blocos;;
    }*/
        double epsilon=0.000001;
    loop(img, h_row_inds, h_col_inds, h_vals, img_w, img_h,epsilon);
    /*for(int i=0; i<100; i++){
        printf("%d %d %f\n", h_row_inds[i], h_col_inds[i], h_vals[i]);
    }*/
//	cudaMemcpy(h_row_inds, dev_row_inds, tlen*sizeof(int), cudaMemcpyDeviceToHost);
    //cudaMemcpy(h_col_inds, dev_col_inds, tlen*sizeof(int), cudaMemcpyDeviceToHost);
//	cudaMemcpy(h_vals, dev_vals, tlen*sizeof(double), cudaMemcpyDeviceToHost);

    lambda=0.0001;
    VectorXd denom(img_w*img_h);
    for(int i=0; i<img_w*img_h; i++){
        denom(i)=img_scr[i]*lambda;
    }

    SparseMatrix<double> mat(img_w*img_h,img_w*img_h);

    std::vector<Triplet<double> > tripletList;
    tripletList.reserve(tlen+img_w*img_h);
    for(int i=0; i<tlen; i++){
        tripletList.push_back(Triplet<double>(h_row_inds[i],h_col_inds[i],h_vals[i]));
    }

    for(int i=0; i<img_w*img_h; i++){
        tripletList.push_back(Triplet<double>(i,i,lambda));
    }
    mat.setFromTriplets(tripletList.begin(), tripletList.end());
    SimplicialCholesky<SparseMatrix<double> > chol(mat);

    VectorXd alpha = chol.solve(denom);
    h_alpha_mtrx=alpha.data();

    double alpha_min, alpha_max, img_min, img_max;
    alpha_min=h_alpha_mtrx[0];
    alpha_max=h_alpha_mtrx[0];
    img_min=img_scr[0];
    img_max=img_scr[0];
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
    for(int i=0; i<img_w*img_h; i++){
        h_alpha_mtrx[i]-=alpha_min;
        h_alpha_mtrx[i]=alpha[i]/alpha_max;
        h_alpha_mtrx[i]=alpha[i]*(img_max-img_min)+img_min;
    }

    Mat alpha_img(img_h, img_w, CV_8UC1);
    ind=0;
    for(int i=0; i<img_w; i++){
        for(int j=0;j<img_h; j++){
            alpha_img.at<uchar>(j,i)=h_alpha_mtrx[ind]*255;
            ind++;
        }
    }
    return alpha_img;
}
