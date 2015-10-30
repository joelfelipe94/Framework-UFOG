#include "colorlines.h"
#include <eigen3/Eigen/Eigenvalues>
#include <eigen3/Eigen/Core>
#include <nlopt.h>

ColorLines::ColorLines()
{
}



int comp_inv(const void * elem1, const void * elem2)
{
    double f = *((double*)elem1);
    double s = *((double*)elem2);
    if (f > s) return  -1;
    if (f < s) return 1;
    return 0;
}

void ColorLines::my_sort(double *v, int *inds,int num_e){
    double val1, val2;
    double temp;
    int temp_ind;
    for(int i=0; i<num_e-2; i++){
        for(int j=1; j<num_e-1; j++){
            if(v[j]>v[j+1]){
                temp=v[j+1];
                temp_ind=inds[j+1];
                v[j+1]=v[j];
                inds[j+1]=inds[j];
                v[j]=temp;
                inds[j]=temp_ind;
            }
        }
    }
}

void ColorLines::patchLine(double lambdas[3], double p1[3], double p2[3], double *img, int h, int w, int c, int min_y, int max_y, int min_x, int max_x, bool onlyPositive){
    int numPx=(max_y-min_y+1)*(max_x-min_x+1);
    double d[numPx];
    double p[c*numPx];
    int ind=0;
    for(int i=min_y; i<=max_y; i++){
        for(int j=min_x; j<=max_x; j++){
            for(int k=0; k<c; k++){
                p[c*ind+k]=img[c*(w*i+j)+k];
            }
            ind++;
        }
    }


    ind=0;
    bool n_zero;
    for(int i=0; i<numPx; i++){
        n_zero=false;
        for(int k=0; k<c; k++){
            if(p[c*i+k]){
                n_zero=true;
            }
        }
        if(n_zero){
            for(int k=0; k<c; k++){
                p[c*ind+k]=p[c*i+k];
            }
            ind++;
        }
    }
    numPx=ind;

    double m[c];

    for(int k=0; k<c; k++){
        m[k]=0;
    }

    for(int i=0; i<numPx; i++){
        for(int k=0; k<c; k++){
            m[k]+=p[c*i+k];
        }
    }

    for(int k=0; k<c; k++){
        m[k]=m[k]/(double)numPx;
    }

    /*if(min_y==5 && min_x==5){
        printf("%f %f %f\n", m[0], m[1], m[2]);
    }*/

    double a[c*numPx];
    for(int i=0; i<numPx; i++){
        for(int k=0; k<c; k++){
            a[c*i+k]=p[c*i+k]-m[k];
        }
    }

    double a2[c*c];

    for(int i=0; i<c; i++){
        for(int j=0; j<c; j++){
            a2[c*i+j]=0;
            for(int k=0; k<numPx; k++){
                a2[c*i+j]+=a[c*k+i]*a[c*k+j];
            }
            a2[c*i+j]=a2[c*i+j]/c;
        }
    }

    /*if(min_y==5 && min_x==5){
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                printf("%f ", a2[3*i+j]);
            }
            printf("\n");
        }
    }*/

    double diag[3];
    double v[3];
    Matrix3d aa(3,3);
    EigenSolver<Matrix3d> es(aa);
    EigenSolver<Eigen::MatrixXd>::EigenvalueType eigenvals;
    EigenSolver<Eigen::MatrixXd>::EigenvectorsType eigenvecs;

    aa << a2[0], a2[1], a2[2], a2[3], a2[4], a2[5],a2[6],a2[7],a2[8];
    eigenvals=es.eigenvalues();
    eigenvecs=es.eigenvectors();
    diag[0]=eigenvals(0,0).real();
    diag[1]=eigenvals(1,0).real();
    diag[2]=eigenvals(2,0).real();
    v[0]=eigenvecs(0,2).real()*0.3;
    v[1]=eigenvecs(1,2).real()*0.3;
    v[2]=eigenvecs(2,2).real()*0.3;

    /*if(min_x==5 && min_y==5){
        std::cout << "matriz:" << std::endl << aa << std::endl;
        std::cout << "eigenvalues:" << std::endl << eigenvals << std::endl;
        std::cout << "eigenvectors:" << std::endl << eigenvecs << std::endl;
        //printf("vectors: %f %f %f\nvalues: %f %f %f\n", v[0], v[1], v[2], diag[0], diag[1], diag[2]);
    }*/
    //printf("%f %f %f\n%f %f %f\n", diag[0],diag[1],diag[2], v[0], v[1], v[2]);
    //calculco dos eigenvalues
    /*double p_val1=a2[1]*a2[1] + a2[2]*a2[2] + a2[5]*a2[5];
    if(p_val1==0){
        diag[0] = a2[0];
        diag[1] = a2[4];
        diag[2] = a2[8];
    } else{
        double q=(a2[0]+a2[4]+a2[8])/3.0;
        double p_val2 = (a2[0]-q)*(a2[0]-q)+(a2[4]-q)*(a2[4]-q)+(a2[8]-q)*(a2[8]-q)+2*p_val1;
        double p_val=sqrt(p_val2/6.0);

        double b[9];
        for(int i=0; i<9; i++){
            b[i]=a2[i];
        }
        for(int i=0; i<9; i+=4){
            b[i]-=q;
        }
        for(int i=0; i<9; i++){
            b[i]=b[i]/p_val;
        }
        double det_b=b[0]*b[4]*b[8]+b[2]*b[3]*b[7]+b[1]*b[5]*b[6];
        det_b-=b[6]*b[4]*b[2]+b[3]*b[1]*b[8]+b[7]*b[5]*b[0];
        double r=det_b/2.0;

        double phi;
        if (r<=-1){
                phi=M_PI/3.0;
        }
        else if(r>=1){
                phi=0;
        }
        else{
                phi=acos(r)/3.0;
        }

        diag[2]=q+2.0*p_val*cos(phi);
        diag[0]=q+2.0*p_val*cos(phi+(2.0*M_PI/3.0));
        diag[1]=3.0*q-diag[2]-diag[0];
    }*/

    for(int i=0; i<3; i++){
        p1[i]=m[i]+v[i];
        p2[i]=m[i]-v[i];
    }

    int sig[3];
    for(int i=0; i<3; i++){
        if(v[i]>0){
            sig[i]=1;
        } else if(v[i]<0){
            sig[i]=-1;
        } else{
            sig[i]=0;
        }
    }
    if(!onlyPositive || !((sig[1]-sig[0]) || sig[2]-sig[0])){
        //printf("entrou\n");
        /*for(int i=0; i<3; i++){
            printf("%f ", p1[i]);
        }
        printf("\n");
        for(int i=0; i<3; i++){
            printf("%f ", p2[i]);
        }
        printf("\n");*/
        for(int i=0; i<3; i++){
            v[i]=v[i]*sig[0];
            p1[i]=m[i]-v[i];
            p2[i]=m[i]+v[i];
        }
        double pq[3*numPx];
        for(int i=0; i<numPx; i++){
            pq[3*i]=p[3*i]-p1[0];
            pq[3*i+1]=p[3*i+1]-p1[1];
            pq[3*i+2]=p[3*i+2]-p1[2];
        }
        double pqxv[3*numPx];
        for(int i=0; i<numPx; i++){
            pqxv[3*i]=pq[3*i+1]*v[2]-pq[3*i+2]*v[1];
            pqxv[3*i+1]=pq[3*i+2]*v[0]-pq[3*i]*v[2];
            pqxv[3*i+2]=pq[3*i]*v[1]-pq[3*i+1]*v[0];
        }

        double norm_v=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
        for(int i=0; i<numPx; i++){
            d[i]=sqrt(pqxv[3*i]*pqxv[3*i]+pqxv[3*i+1]*pqxv[3*i+1]+pqxv[3*i+2]*pqxv[3*i+2])/norm_v;
        }
        int inds[numPx];
        for(int i=0; i<numPx; i++){
            inds[i]=i;
        }
        my_sort(d, inds, numPx);
        double p_sorted[3*numPx];
        for(int i=0; i<numPx; i++){
            p_sorted[3*i]=p[3*inds[i]];
            p_sorted[3*i+1]=p[3*inds[i]+1];
            p_sorted[3*i+2]=p[3*inds[i]+2];
        }

        numPx = 0.8*numPx+0.5;

        for(int k=0; k<c; k++){
            m[k]=0;
        }

        for(int i=0; i<numPx; i++){
            for(int k=0; k<c; k++){
                m[k]+=p_sorted[c*i+k];
            }
        }

        for(int k=0; k<c; k++){
            m[k]=m[k]/(double)numPx;
        }

        for(int i=0; i<numPx; i++){
            for(int k=0; k<c; k++){
                a[c*i+k]=p_sorted[c*i+k]-m[k];
            }
        }

        for(int i=0; i<c; i++){
            for(int j=0; j<c; j++){
                a2[c*i+j]=0;
                for(int k=0; k<numPx; k++){
                    a2[c*i+j]+=a[c*k+i]*a[c*k+j];
                }
                a2[c*i+j]=a2[c*i+j]/c;
            }
        }

        /*for(int i=0; i<9; i++){
            aa[i/3][i%3]=a2[i];
        }*/
        //printf("entrou aqui!\n");

        aa << a2[0], a2[1], a2[2], a2[3], a2[4], a2[5],a2[6],a2[7],a2[8];
        es.compute(aa);
        eigenvals=es.eigenvalues();
        eigenvecs=es.eigenvectors();
        diag[0]=eigenvals(0,0).real();
        diag[1]=eigenvals(1,0).real();
        diag[2]=eigenvals(2,0).real();
        v[0]=eigenvecs(0,2).real()*0.3;
        v[1]=eigenvecs(1,2).real()*0.3;
        v[2]=eigenvecs(2,2).real()*0.3;

        /*if(min_x==5 && min_y==5){
            std::cout << "The eigenvalues of A are:" << std::endl << eigenvals << std::endl;
            std::cout << "The eigenvectors of A are:" << std::endl << eigenvecs << std::endl;
            printf("vectors: %f %f %f\nvalues: %f %f %f\n", v[0], v[1], v[2], diag[0], diag[1], diag[2]);
        }*/
        //printf("%.20f %.20f %.20f\n%f %f %f\n", diag[0],diag[1],diag[2], v[0], v[1], v[2]);
        for(int i=0; i<3; i++){
            p1[i]=m[i]+v[i];
            p2[i]=m[i]-v[i];
        }

        norm_v=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
        double cross_vp1[3];
        cross_vp1[0]=v[1]*(-p1[2])-v[2]*(-p1[1]);
        cross_vp1[1]=v[2]*(-p1[0])-v[0]*(-p1[2]);
        cross_vp1[2]=v[0]*(-p1[1])-v[1]*(-p1[0]);

        //printf("%f %f %f\n", cross_vp1[0], cross_vp1[1], cross_vp1[2]);

        double distFromOrigin = sqrt(cross_vp1[0]*cross_vp1[0]+cross_vp1[1]*cross_vp1[1]+cross_vp1[2]*cross_vp1[2])/norm_v;

        lambdas[0]=diag[2];
        lambdas[1]=diag[2]/(diag[1]+0.00000001);
        lambdas[2]=distFromOrigin;

        //printf("%f %.20f %f %f %f\n", diag[2], diag[1], lambdas[0], lambdas[1], lambdas[2]);
        for(int i=0; i<3; i++){
            if(v[i]>0){
                sig[i]=1;
            } else if(v[i]<0){
                sig[i]=-1;
            } else{
                sig[i]=0;
            }
        }

        if(onlyPositive && (sig[1]-sig[0]+sig[2]-sig[0])!=0){
                lambdas[0]=0;
            lambdas[1]=0;
            lambdas[2]=0;
            }
        //printf("%f\n",distFromOrigin);
        //printf("%f %f\n",norm_v, distFromOrigin);
        /*for(int i=0; i<3; i++){
            printf("%f ", p1[i]);
        }
        printf("\n");
        for(int i=0; i<3; i++){
            printf("%f ", p2[i]);
        }*/
        /*for(int i=0; i<numClose; i++){
            printf("%f %f %f\n", p_sorted[3*i],p_sorted[3*i+1],p_sorted[3*i+2]);
        }*?
        /*printf("%f\n", norm_v);
        for(int i=0; i<numPx; i++){
            printf("%f\n", d[i]);
        }*/
        /*for(int i=0; i<3; i++){
            printf("%f ", v[i]);
        }
        printf("\n");
        for(int i=0; i<3; i++){
            printf("%f ", p1[i]);
        }
        printf("\n");
        for(int i=0; i<3; i++){
            printf("%f ", p2[i]);
        }*/
    } else {
        lambdas[0]=0;
        lambdas[1]=0;
        lambdas[2]=0;
    }

    /*for(int i=0; i<3; i++){
        printf("%d\n", sig[i]);
    }*/
    /*for(int i=0; i<c; i++){
        printf("%f\n", m[i]);
    }*/
    //printf("%d\n\n", numPx);
    /*for(int i=0; i<c; i++){
        for(int k=0; k<c; k++){
            printf("%f ", a2[c*i+k]);
        }
        printf("\n");
    }*/
}


Vec3f ColorLines::findOrientation(const Mat *m)
{


    int w=m->cols;
    int h=m->rows;
    int max_dim=max(w,h);
    if(max_dim<600){
        double factor=max(600.0/(double)max_dim,2.0);
        resize(*m, *m, Size(), factor, factor, INTER_LINEAR);
    }
    w=m->cols;
    h=m->rows;
    Mat m_gray, edges;
    m->convertTo(m_gray, CV_8UC1);
        cvtColor(*m, m_gray, CV_BGR2GRAY);
    //nao sei como colocar esses parametros, o matlab passa um parametro so e eu nao sei como traduzir isso pro opencv
    //double otsu_thresh_val = cv::threshold(m_gray, edges, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    //Canny(m_gray, edges, otsu_thresh_val*0.5, otsu_thresh_val);
    //medida temporaria para dar o mesmo resultado do matlab, que é uma imagem toda preta
    Canny(m_gray, edges, 255*1, 255*1);
    //namedWindow( "Canny", WINDOW_AUTOSIZE );
        //imshow("Canny",edges);
    waitKey();
    int n=10;
    int indLocs[w*h][2];
    int cont=0;
    int x, y;
    uchar *edges_data=edges.data;
    int flagbreak=0;
    for(int i=0; i<=h-n; i++){
        for(int j=0; j<=w-n; j++){
            flagbreak=0;
            for(int i2=i; i2<i+n-1; i2++){
                for(int j2=j; j2<j+n-1; j2++){
                    if(edges_data[w*i2+j2]){
                        flagbreak=1;
                        break;
                    }
                }
            }
            if(!flagbreak){
                indLocs[cont][0]=i;
                indLocs[cont][1]=j;
                cont++;
            }
        }
    }
    double *l=(double*)malloc(cont*3*sizeof(double));
    double *p=(double*)malloc(cont*6*sizeof(double));
    int *locs=(int*)malloc(cont*2*sizeof(int));
    int c;

    if(m->type()==CV_8UC3){
        c=3;
    } else if(m->type()==CV_8UC1){
        c=1;
    }

    uchar *m_data=m->data;
    double *img=(double*)malloc(w*h*c*sizeof(double));
    for(int i=0; i<w*h*c; i++){
        img[i]=(double)m_data[i]/255.0l;
    }


    double lambdas[3];
    double p1[3];
    double p2[3];

    patchLine(lambdas, p1, p2, img, h, w, c, 0, 0+n-1, 3, 3+n-1, true);
    //printf("%f %f %f\n", lambdas[0], lambdas[1], lambdas[2]);
    for(int i=0; i<cont; i++){
            y = indLocs[i][0];
            x = indLocs[i][1];
        patchLine(lambdas, p1, p2, img, h, w, c, y, y+n-1, x, x+n-1, true);
        //if(i==3){
        //	printf("%d %d %f %f %f\n", y, x, lambdas[0], lambdas[1], lambdas[2]);
        //}
        if(lambdas[0]>0.00001 || lambdas[1]>0.00001 || lambdas[2]>0.00001){
            l[3*i]=lambdas[0];
            l[3*i+1]=lambdas[1];
            l[3*i+2]=lambdas[2];
            p[6*i]=p1[0];
            p[6*i+1]=p1[1];
            p[6*i+2]=p1[2];
            p[6*i+3]=p2[0];
            p[6*i+4]=p2[1];
            p[6*i+5]=p2[2];
            locs[2*i]=y;
            locs[2*i+1]=x;
        } else{
            l[3*i]=0;
            l[3*i+1]=0;
            l[3*i+2]=0;
            p[6*i]=0;
            p[6*i+1]=0;
            p[6*i+2]=0;
            p[6*i+3]=0;
            p[6*i+4]=0;
            p[6*i+5]=0;
            locs[2*i]=0;
            locs[2*i+1]=0;
        }
    }

    int ind=0;
    for(int i=0; i<cont; i++){
        if(l[3*i] || l[3*i+1] || l[3*i+2]){
            l[3*ind]=l[3*i];
            l[3*ind+1]=l[3*i+1];
            l[3*ind+2]=l[3*i+2];
            p[6*ind]=p[6*i];
            p[6*ind+1]=p[6*i+1];
            p[6*ind+2]=p[6*i+2];
            p[6*ind+3]=p[6*i+3];
            p[6*ind+4]=p[6*i+4];
            p[6*ind+5]=p[6*i+5];
            locs[2*ind]=locs[2*i];
            locs[2*ind+1]=locs[2*i+1];
            ind++;
        }
    }
    int num_e=ind;

    double *n_vec=(double*)malloc(3*num_e*sizeof(double));
    double n_norm;
    for(int i=0; i<num_e; i++){
        n_vec[3*i]=p[6*i+1]*p[6*i+5]-p[6*i+2]*p[6*i+4];
        n_vec[3*i+1]=p[6*i+2]*p[6*i+3]-p[6*i]*p[6*i+5];
        n_vec[3*i+2]=p[6*i]*p[6*i+4]-p[6*i+1]*p[6*i+3];
        n_norm=sqrt(n_vec[3*i]*n_vec[3*i]+n_vec[3*i+1]*n_vec[3*i+1]+n_vec[3*i+2]*n_vec[3*i+2]);
        n_vec[3*i]=n_vec[3*i]/n_norm;
        n_vec[3*i+1]=n_vec[3*i+1]/n_norm;
        n_vec[3*i+2]=n_vec[3*i+2]/n_norm;
    }

    double *l1, *l2, *l3;
    l1=(double*)malloc(num_e*sizeof(double));
    l2=(double*)malloc(num_e*sizeof(double));
    l3=(double*)malloc(num_e*sizeof(double));

    for(int i=0; i<num_e; i++){
        l1[i]=l[3*i];
        l2[i]=l[3*i+1];
        l3[i]=l[3*i+2];
    }

    qsort(l1, num_e, sizeof(double), comp_inv);
    qsort(l2, num_e, sizeof(double), comp_inv);
    qsort(l3, num_e, sizeof(double), comp_inv);

    double thresh[3]={l1[49], l2[49], l3[49]};
    int *diff=(int*)malloc(num_e*sizeof(int));

    for(int i=0; i<num_e; i++){
        diff[i]=(l[3*i]>=thresh[0])+(l[3*i+1]>=thresh[1])+(l[3*i+2]>=thresh[2]);
    }
    int *goodInds=(int*)malloc(num_e*sizeof(int));
    ind=0;
    for(int i=0; i<num_e; i++){
        if(diff[i]==3){
            goodInds[ind]=i;
            ind++;
        }
    }
    int nGood=ind;
    bool found=false;
    double ang=15;
    double othresh[3]={thresh[0], thresh[1], thresh[2]};
    int *good=(int*)malloc(num_e*sizeof(int));

    double *approvedNormals=(double*)malloc(3*num_e*sizeof(double));
        double *approvedPoints=(double*)malloc(6*num_e*sizeof(double));
        int *approvedLocations=(int*)malloc(2*num_e*sizeof(int));
    double *normals=(double*)malloc(3*num_e*sizeof(double));
        double *points=(double*)malloc(6*num_e*sizeof(double));
        int *locations=(int*)malloc(2*num_e*sizeof(int));
    int sum_good;
    int size_approved=0;
    double dot;
    int sizeGoodInds;
    while(!found){
        ang--;
        thresh[0]=othresh[0];
        thresh[1]=othresh[1];
        thresh[2]=othresh[2];
        for(int i=0; i<num_e; i++){
            good[i]=1;
        }
        sum_good=num_e;
        while(nGood<10 && sum_good>0){
            thresh[0]=thresh[0]*0.97;
            thresh[1]=thresh[1]*0.97;
            thresh[2]=thresh[2]*0.97;
            for(int i=0; i<num_e; i++){
                diff[i]=(l[3*i]>=thresh[0])+(l[3*i+1]>=thresh[1])+(l[3*i+2]>=thresh[2])+good[i];
            }
            ind=0;
            for(int i=0; i<num_e; i++){
                if(diff[i]==4){
                    goodInds[ind]=i;
                    ind++;
                }
            }
            sizeGoodInds=ind;
            nGood=sizeGoodInds+size_approved;
            if(nGood>=10){
                ind=0;
                for(int i=0; i<sizeGoodInds; i++){
                    normals[3*ind]=n_vec[3*goodInds[i]];
                    normals[3*ind+1]=n_vec[3*goodInds[i]+1];
                    normals[3*ind+2]=n_vec[3*goodInds[i]+2];
                    points[6*ind]=p[6*goodInds[i]];
                    points[6*ind+1]=p[6*goodInds[i]+1];
                    points[6*ind+2]=p[6*goodInds[i]+2];
                    points[6*ind+3]=p[6*goodInds[i]+3];
                    points[6*ind+4]=p[6*goodInds[i]+4];
                    points[6*ind+5]=p[6*goodInds[i]+5];
                    locations[2*ind]=locs[2*goodInds[i]];
                    locations[2*ind+1]=locs[2*goodInds[i]+1];
                    ind++;
                }
                for(int i=0; i<sizeGoodInds-1; i++){
                    for(int j=i+1; j<sizeGoodInds; j++){
                        dot=normals[3*i]*normals[3*j]+normals[3*i+1]*normals[3*j+1]+normals[3*i+2]*normals[3*j+2];
                        if(dot>cos(ang*M_PI/180)){
                            good[goodInds[j]]=0;
                            //apaga a posição do vetor
                            for(int k=j; k<sizeGoodInds-1; k++){
                                normals[3*k]=n_vec[3*(k+1)];
                                normals[3*k+1]=n_vec[3*(k+1)+1];
                                normals[3*k+2]=n_vec[3*(k+1)+2];
                                points[6*k]=p[6*(k+1)];
                                points[6*k+1]=p[6*(k+1)+1];
                                points[6*k+2]=p[6*(k+1)+2];
                                points[6*k+3]=p[6*(k+1)+3];
                                points[6*k+4]=p[6*(k+1)+4];
                                points[6*k+5]=p[6*(k+1)+5];
                                locations[2*k]=locs[2*(k+1)];
                                locations[2*k+1]=locs[2*(k+1)+1];
                                goodInds[k]=goodInds[k+1];
                            }
                            sizeGoodInds--;
                        }
                    }
                }
                for(int i=0; i<sizeGoodInds; i++){
                    approvedNormals[3*size_approved]=normals[i];
                    approvedNormals[3*size_approved+1]=normals[i+1];
                    approvedNormals[3*size_approved+2]=normals[i+2];
                    approvedPoints[6*size_approved]=points[i];
                    approvedPoints[6*size_approved+1]=points[i+1];
                    approvedPoints[6*size_approved+2]=points[i+2];
                    approvedPoints[6*size_approved+3]=points[i+3];
                    approvedPoints[6*size_approved+4]=points[i+4];
                    approvedPoints[6*size_approved+5]=points[i+5];
                    approvedLocations[2*size_approved]=locations[2*i];
                    approvedLocations[2*size_approved+1]=locations[2*i+1];
                    size_approved++;
                }
                for(int i=0; i<sizeGoodInds; i++){
                    dot=n_vec[3*i]*normals[3*i]+n_vec[3*i+1]*normals[3*i+1]+n_vec[3*i+2]*normals[3*i+2];
                    good[i]=good[i] && (dot<=cos(ang*M_PI/180));
                }
                nGood=size_approved;
            }
        }
        if(nGood>=10){
            found=true;
        }
    }
    double *as=(double*)malloc(3*((nGood*(nGood-1))/2)*sizeof(double));
    for(int i=0; i<3*((nGood*(nGood-1))/2); i++){
        as[i]=0;
    }
    double curA[3];
    int sig[3];
    double norm;
    cont=0;
    for(int i=0; i<nGood; i++){
        for(int j=i+1; j<nGood-1; j++){
            curA[0]=approvedNormals[3*i+1]*approvedNormals[3*j+2]-approvedNormals[3*i+2]*approvedNormals[3*j+1];
            curA[1]=approvedNormals[3*i+2]*approvedNormals[3*j]-approvedNormals[3*i]*approvedNormals[3*j+2];
            curA[2]=approvedNormals[3*i]*approvedNormals[3*j+1]-approvedNormals[3*i+1]*approvedNormals[3*j];
            for(int i=0; i<3; i++){
                if(curA[i]>0){
                    sig[i]=1;
                } else if(curA[i]<0){
                    sig[i]=-1;
                } else{
                    sig[i]=0;
                }
            }
            norm=sqrt(curA[0]*curA[0]+curA[1]*curA[1]+curA[2]*curA[2]);
            if(norm>0.0000001 && !((sig[1]-sig[0]) || (sig[2]-sig[0]))){
                curA[0]=sig[0]*curA[0]/norm;
                curA[1]=sig[0]*curA[1]/norm;
                curA[2]=sig[0]*curA[2]/norm;
                as[3*cont]=curA[0];
                as[3*cont+1]=curA[1];
                as[3*cont+2]=curA[2];
                cont++;
            }
        }
    }

    int nas=cont;
    double *distances=(double*)malloc(nGood*nas*sizeof(double));
    for(int i=0; i<nGood*nas; i++){
        distances[i]=0;
    }
    double ori[3];
    double oriXA[3];
    for(int i=0; i<nas; i++){
        for(int j=0; j<nGood; j++){
            ori[0]=approvedPoints[j*6+3]-approvedPoints[j*6];
            ori[1]=approvedPoints[j*6+4]-approvedPoints[j*6+1];
            ori[2]=approvedPoints[j*6+5]-approvedPoints[j*6+2];
            oriXA[0]=as[3*i+1]*ori[2]-as[3*i+2]*ori[1];
            oriXA[1]=as[3*i+2]*ori[0]-as[3*i]*ori[2];
            oriXA[2]=as[3*i]*ori[1]-as[3*i+1]*ori[0];
            dot=approvedPoints[j*6]*oriXA[0]+approvedPoints[j*6+1]*oriXA[1]+approvedPoints[j*6+2]*oriXA[2];
            norm=sqrt(oriXA[0]*oriXA[0]+oriXA[1]*oriXA[1]+oriXA[2]*oriXA[2]);
            distances[nGood*i+j]=fabs(dot/norm);
        }
    }

    for(int i=0; i<nas; i++){
        qsort(&distances[nGood*i], nGood, sizeof(double), comp_inv);
    }

    double *meds=(double*)malloc(nas*sizeof(double));
    for(int i=0; i<nas; i++){
        meds[i]=0;
        for(int j=0; j<nGood-2; j++){
            meds[i]+=distances[i*nGood+j];
        }
        meds[i]=meds[i]/(double)(nGood-2);
    }

    double min_meds=meds[0];
    ind=0;
    for(int i=1; i<nas; i++){
        if(meds[i]<min_meds){
            min_meds=meds[i];
            ind=i;
        }
    }

    double *result=(double*)malloc(3*sizeof(double));
    result[0]=as[3*ind];
    result[1]=as[3*ind+1];
    result[2]=as[3*ind+2];
    /*for(int i=0; i<nas; i++){
        for(int j=0; j<nGood; j++){
            printf("%f ", distances[i*nGood+j]);
        }
        printf("\n");
    }*/
    /*for(int i=0; i<num_e; i++){
        if(diff[i]>0){
            printf("%d\n", diff[i]);
        }
    }*/

    //patchLine(I(1:1+n-1,1:1+n-1,:),true);
    //patchLine(lambdas, p1, p2, img, h, w, c, 0, 0+n-1, 0, 0+n-1, true);
    //printf("%f %f %f\n", lambdas[0], lambdas[1], lambdas[2]);
    //printf("%f %f %f\n", p1[0], p1[1], p1[2]);
    //printf("%f %f %f\n", p2[0], p2[1], p2[2]);
    /*printf("%d\n", cont);
    for(int i=0; i<100; i++){
        //printf("%d %d\n", indLocs[i][0]+1,indLocs[i][1]+1);
    }*/
    free(meds);
    free(as);
    free(distances);
    free(approvedNormals);
    free(approvedPoints);
    free(approvedLocations);
    free(normals);
    free(points);
    free(locations);
    free(good);
    free(goodInds);
    free(diff);
    free(l);
    free(p);
    free(locs);
    free(img);
    free(n_vec);
    free(l1);
    free(l2);
    free(l3);

    return Vec3f(result[0],result[1],result[2]);


}



int comp(const void * elem1, const void * elem2)
{
    double f = *((double*)elem1);
    double s = *((double*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

typedef struct {
    double *withoutA; double *alpha; double *mshading; int h; int w; int c; int numBins; double initMag;
} struct_fitError;

double fitError(unsigned n, const double *ak, double *grad, void *entrada){
    struct_fitError *e = (struct_fitError *) entrada;
    double *alpha=e->alpha;
    double *mshading=e->mshading;
    int h=e->h;
    int w=e->w;
    int c=e->c;
    int numBins=e->numBins;
    double initMag=e->initMag;
    double a=ak[0];
    double k=ak[1];
    double *transmission=(double*)malloc(h*w*sizeof(double));
    for(int i=0; i<h*w; i++){
        transmission[i]=1-a*alpha[i]/initMag;
    }
    double max_alpha=transmission[0];
    double min_alpha=transmission[0];
    for(int i=0; i<w*h; i++){
        if(transmission[i]>max_alpha){
            max_alpha=transmission[i];

        } else if(transmission[i]<min_alpha){
            min_alpha=transmission[i];
        }
    }
    double mtrans[numBins+1];
    double binW=(max_alpha-min_alpha)/(double)numBins;
    mtrans[0]=min_alpha;
    mtrans[numBins]=max_alpha;
    for(int i=1; i<numBins; i++){
        mtrans[i]=mtrans[i-1]+binW;
    }
    double sumErr=0;
    double t;
    for(int i=0; i<numBins; i++){
        if(mshading[i]>0){
            t=mtrans[i];
            sumErr+=fabs(mshading[i]-fabs((k*a*t)/(a+t-1)));
        }
    }
    free(transmission);
    //printf("%f %f %f\n", ak[0], ak[1], sumErr);
    return sumErr;
}

double ColorLines::findMagnitude(const Mat *img, Mat *alpha_mat, double a[3])
{
    int w=img->cols;
    int h=img->rows;
    int t=img->type();
    int c;


    if(img->type()==CV_8UC3){
        c=3;
    } else if(img->type()==CV_8UC1){
        c=1;
    }
    uchar *p_data=img->data;
    uchar *alpha_data=alpha_mat->data;

    //cout << *alpha_mat << endl;
    double img_div[h*w*c];
    double *alpha=(double*)malloc(h*w*sizeof(double));

    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            alpha[i*w+j]=(double)alpha_data[w*i+j]/255.0l;
            for(int k=0; k<c; k++){
                img_div[c*(w*i+j)+k]=(double(p_data[c*(w*i+j)+k])/255.0l)/a[k];
            }
        }
    }



    double *withoutA = (double*)malloc(h*w*c*sizeof(double));

    double temp;
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            for(int k=0; k<c; k++){


                temp=(double(p_data[c*(w*i+j)+k])/255.0l)-alpha[w*i+j]*a[k];

                if(temp>0){
                    withoutA[c*(w*i+j)+k]=temp;
                } else{
                    withoutA[c*(w*i+j)+k]=0;
                }
            }
        }
    }


    double *gray=(double*)malloc(w*h*sizeof(double));
    double initMag=0.5;
    bool isNegative=true;

    while(isNegative){
            initMag+=0.1;
        isNegative=false;
        for(int i=0; i<h; i++){
            for(int j=0; j<w; j++){
                gray[w*i+j]=0;
                for(int k=0; k<c; k++){
                    temp=withoutA[c*(w*i+j)+k]/(1-alpha[w*i+j]/initMag);
                    isNegative=temp<0;
                    gray[w*i+j]+=temp*temp;
                }
                gray[w*i+j]=sqrt(gray[w*i+j]);

            }
        }
    }


    double *transmission=(double*)malloc(w*h*sizeof(double));
    for(int i=0; i<w*h; i++){
        transmission[i]=1-alpha[i]/initMag;
    }

    double alpha_min=transmission[0];
    double alpha_max=transmission[0];
    for(int i=1; i<w*h; i++){
        if(transmission[i]>alpha_max){
            alpha_max=transmission[i];
        } else if(transmission[i]<alpha_min){
            alpha_min=transmission[i];
        }

    }

    int numBins=50;
    double binW=(alpha_max-alpha_min)/(double)numBins;
    double mtrans[numBins+1];
    double mshading[numBins+1];
    mtrans[0]=alpha_min;
    mshading[0]=0;
    mtrans[numBins]=alpha_max;
    for(int i=1; i<numBins; i++){
        mtrans[i]=mtrans[i-1]+binW;
        mshading[i]=0;
    }

    double val;
    double *inBin=(double*)malloc(w*h*sizeof(double));
    int ind;

    for(int i=0; i<=numBins; i++){
        ind=0;
        val=mtrans[i]+binW/2.0;
        for(int j=0; j<w*h; j++){
            if(transmission[j]<val+binW/2.0 && transmission[j]>=val-binW/2.0){
                inBin[ind]=gray[j];
                ind++;
            }
        }
        if(ind>100){
            qsort(inBin, ind, sizeof(double), comp);
            mshading[i]=inBin[(int)(0.995*ind+0.5)-1];
        }
    }
    double ak[2];
    ak[0]=1;
    ak[1]=1;
    struct_fitError entrada;
    entrada.withoutA=withoutA;
    entrada.alpha=alpha;
    entrada.mshading=mshading;
    entrada.h=w;
    entrada.w=h;
    entrada.c=c;
    entrada.numBins=numBins;
    entrada.initMag=initMag;

    cout << " OPTIMIZATION " << endl;
    nlopt_opt opt;

    //algoritmos de minimização, nao sei qual e melhor..
    opt = nlopt_create(NLOPT_GN_DIRECT_L, 2);
    //opt = nlopt_create(NLOPT_GN_DIRECT, 2);
    //opt = nlopt_create(NLOPT_GN_CRS2_LM, 2);
    //opt = nlopt_create(NLOPT_GD_STOGO, 2);
    //opt = nlopt_create( NLOPT_GD_STOGO_RAND, 2);
    //opt = nlopt_create(NLOPT_GN_ISRES, 2);
    //opt = nlopt_create(NLOPT_GN_ESCH, 2);

    //valores maximos e minimos do espaço de busca, nao sei se 0 e 1 sao valores bons
    double lb[2]={0,0};
    double ub[2]={1,1};
    nlopt_set_lower_bounds(opt, lb);
    nlopt_set_upper_bounds(opt, ub);
    nlopt_set_min_objective(opt, fitError, &entrada);
    //acho que isso esta relacionado com a precisao, se aumentar o resultado fica ruim, se diminuir muito nao converge nunca
    nlopt_set_xtol_rel(opt, 1e-1);

    //numero maximo de iterações, pode ser que precise aumentar
    nlopt_set_maxeval(opt, 500);

    double minf;

    if (nlopt_optimize(opt, ak, &minf) < 0) {
            printf("nlopt failed!\n");
    }
    double mag=initMag/ak[0];
    free(alpha);
    free(gray);
    free(inBin);
    free(transmission);
    //printf("resultado final: %f %f %f\n", ak[0], ak[1], mag);
    return mag;

}
