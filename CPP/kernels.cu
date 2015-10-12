#define WINSIZE 1
const int nebsize=(WINSIZE*2+1)*(WINSIZE*2+1);

__global__ void loop(double *img, int *row_inds, int *col_inds, double *vals, int w, int h, double epsilon){
    int ind=blockIdx.x*blockDim.x+threadIdx.x;
    if(ind<w*h && ind%h>=WINSIZE && ind%h<h-WINSIZE && ind/h>=WINSIZE && ind/h<w-WINSIZE){
        int win_inds[nebsize];
        double tvals[nebsize*nebsize];
        int k;
        k=0;
        for(int i=-WINSIZE; i<=WINSIZE; i++){
            for(int j=-WINSIZE; j<=WINSIZE; j++){
                win_inds[k]=ind+i*h+j;
                k++;
            }
        }
        double winI[3*nebsize];
        double winI_aux[3*nebsize];
        double pre_win_var[9];
        double win_var[9];
        double win_mu[3];
        double detwin;
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
        int base=((ind/h-WINSIZE)*(h-2*WINSIZE)+(ind%h-WINSIZE))*nebsize*nebsize;
        for(int i=0; i<nebsize; i++){
            for(int j=0; j<nebsize; j++){
                row_inds[base+k]=win_inds[j];
                col_inds[base+k]=win_inds[i];
                k++;
            }
        }

        memcpy(&vals[base],tvals, nebsize*nebsize*sizeof(double));
    }
}
