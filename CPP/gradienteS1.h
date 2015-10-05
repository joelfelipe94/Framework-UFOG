#ifndef GRADIENTES1_H
#define GRADIENTES1_H
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include<time.h>
#define SIGN(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

using namespace cv;




static double PYTHAG(double a, double b)
{
    double at = fabs(a), bt = fabs(b), ct, result;

    if (at > bt)       { ct = bt / at; result = at * sqrt(1.0 + ct * ct); }
    else if (bt > 0.0) { ct = at / bt; result = bt * sqrt(1.0 + ct * ct); }
    else result = 0.0;
    return(result);
}



/*
*   Transformacao de uma matrix A m x 2 em udv, matrizes de
*	transformação ortogonal esquerda e direita e d é uma
*	matriz diagonal de valores singulares
*/
int dsvd(float a[][2], int m, int n, float w[2], float v[][2])
{
    int flag, i, its, j, jj, k, l, nm;
    double c, f, h, s, x, y, z;
    double anorm = 0.0, g = 0.0, scale = 0.0;
    double *rv1;

    if (m < n)
    {
        fprintf(stderr, "#rows must be > #cols \n");
        return(0);
    }

    rv1 = (double *)malloc((unsigned int) n*sizeof(double));

    /* Householder redução para forma bidiagonal */
    for (i = 0; i < n; i++)
    {
        /* redução pela esquerda */
        l = i + 1;
        rv1[i] = scale * g;
        g = s = scale = 0.0;
        if (i < m)
        {
            for (k = i; k < m; k++)
                scale += fabs((double)a[k][i]);
            if (scale)
            {
                for (k = i; k < m; k++)
                {
                    a[k][i] = (float)((double)a[k][i]/scale);    //aplica normalização
                    s += ((double)a[k][i] * (double)a[k][i]);    //quadrado do valor singular
                }
                f = (double)a[i][i];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                a[i][i] = (float)(f - g);
                if (i != n - 1)
                {
                    for (j = l; j < n; j++)
                    {
                        for (s = 0.0, k = i; k < m; k++)
                            s += ((double)a[k][i] * (double)a[k][j]);
                        f = s / h;
                        for (k = i; k < m; k++)
                            a[k][j] += (float)(f * (double)a[k][i]);
                    }
                }
                for (k = i; k < m; k++)
                    a[k][i] = (float)((double)a[k][i]*scale);
            }
        }
        w[i] = (float)(scale * g);

        /* reducao pela direita */
        g = s = scale = 0.0;
        if (i < m && i != n - 1)
        {
            for (k = l; k < n; k++)
                scale += fabs((double)a[i][k]);
            if (scale)
            {
                for (k = l; k < n; k++)
                {
                    a[i][k] = (float)((double)a[i][k]/scale);
                    s += ((double)a[i][k] * (double)a[i][k]);
                }
                f = (double)a[i][l];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                a[i][l] = (float)(f - g);
                for (k = l; k < n; k++)
                    rv1[k] = (double)a[i][k] / h;
                if (i != m - 1)
                {
                    for (j = l; j < m; j++)
                    {
                        for (s = 0.0, k = l; k < n; k++)
                            s += ((double)a[j][k] * (double)a[i][k]);
                        for (k = l; k < n; k++)
                            a[j][k] += (float)(s * rv1[k]);
                    }
                }
                for (k = l; k < n; k++)
                    a[i][k] = (float)((double)a[i][k]*scale);
            }
        }
        anorm = MAX(anorm, (fabs((double)w[i]) + fabs(rv1[i])));
    }

    /*a cumula a tranformacao pela direita */
    for (i = n - 1; i >= 0; i--)
    {
        if (i < n - 1)
        {
            if (g)
            {
                for (j = l; j < n; j++)
                    v[j][i] = (float)(((double)a[i][j] / (double)a[i][l]) / g);
                /* double division to avoid underflow */
                for (j = l; j < n; j++)
                {
                    for (s = 0.0, k = l; k < n; k++)
                        s += ((double)a[i][k] * (double)v[k][j]);
                    for (k = l; k < n; k++)
                        v[k][j] += (float)(s * (double)v[k][i]);
                }
            }
            for (j = l; j < n; j++)
                v[i][j] = v[j][i] = 0.0;
        }
        v[i][i] = 1.0;
        g = rv1[i];
        l = i;
    }

    /* acumula a transformacao pela esquerda */
    for (i = n - 1; i >= 0; i--)
    {
        l = i + 1;
        g = (double)w[i];
        if (i < n - 1)
            for (j = l; j < n; j++)
                a[i][j] = 0.0;
        if (g)
        {
            g = 1.0 / g;
            if (i != n - 1)
            {
                for (j = l; j < n; j++)
                {
                    for (s = 0.0, k = l; k < m; k++)
                        s += ((double)a[k][i] * (double)a[k][j]);
                    f = (s / (double)a[i][i]) * g;
                    for (k = i; k < m; k++)
                        a[k][j] += (float)(f * (double)a[k][i]);
                }
            }
            for (j = i; j < m; j++)
                a[j][i] = (float)((double)a[j][i]*g);
        }
        else
        {
            for (j = i; j < m; j++)
                a[j][i] = 0.0;
        }
        ++a[i][i];
    }

    /* diagonaliza a forma bidiagonal */
    for (k = n - 1; k >= 0; k--)
    {                             /* loop sobre os valores singulares*/
        for (its = 0; its < 30; its++)
        {                         /* loop sobre as iteracoes */
            flag = 1;
            for (l = k; l >= 0; l--)
            {                     /* test for splitting */
                nm = l - 1;
                if (fabs(rv1[l]) + anorm == anorm)
                {
                    flag = 0;
                    break;
                }
                if (fabs((double)w[nm]) + anorm == anorm)
                    break;
            }
            if (flag)
            {
                c = 0.0;
                s = 1.0;
                for (i = l; i <= k; i++)
                {
                    f = s * rv1[i];
                    if (fabs(f) + anorm != anorm)
                    {
                        g = (double)w[i];
                        h = PYTHAG(f, g);
                        w[i] = (float)h;
                        h = 1.0 / h;
                        c = g * h;
                        s = (- f * h);
                        for (j = 0; j < m; j++)
                        {
                            y = (double)a[j][nm];
                            z = (double)a[j][i];
                            a[j][nm] = (float)(y * c + z * s);
                            a[j][i] = (float)(z * c - y * s);
                        }
                    }
                }
            }
            z = (double)w[k];
            if (l == k)
            {                  /* convergence */
                if (z < 0.0)
                {              /* make singular value nonnegative */
                    w[k] = (float)(-z);
                    for (j = 0; j < n; j++)
                        v[j][k] = (-v[j][k]);
                }
                break;
            }
            if (its >= 30) {
                free((void*) rv1);
                fprintf(stderr, "No convergence after 30,000! iterations \n");
                return(0);
            }

            /* shift from bottom 2 x 2 minor */
            x = (double)w[l];
            nm = k - 1;
            y = (double)w[nm];
            g = rv1[nm];
            h = rv1[k];
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
            g = PYTHAG(f, 1.0);
            f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;

            /* next QR transformation */
            c = s = 1.0;
            for (j = l; j <= nm; j++)
            {
                i = j + 1;
                g = rv1[i];
                y = (double)w[i];
                h = s * g;
                g = c * g;
                z = PYTHAG(f, h);
                rv1[j] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y = y * c;
                for (jj = 0; jj < n; jj++)
                {
                    x = (double)v[jj][j];
                    z = (double)v[jj][i];
                    v[jj][j] = (float)(x * c + z * s);
                    v[jj][i] = (float)(z * c - x * s);
                }
                z = PYTHAG(f, h);
                w[j] = (float)z;
                if (z)
                {
                    z = 1.0 / z;
                    c = f * z;
                    s = h * z;
                }
                f = (c * g) + (s * y);
                x = (c * y) - (s * g);
                for (jj = 0; jj < m; jj++)
                {
                    y = (double)a[jj][j];
                    z = (double)a[jj][i];
                    a[jj][j] = (float)(y * c + z * s);
                    a[jj][i] = (float)(z * c - y * s);
                }
            }
            rv1[l] = 0.0;
            rv1[k] = f;
            w[k] = (float)x;
        }
    }
    free((void*) rv1);
    return(1);
}

Mat gradienteS1(Mat imagemOriginal,int p=9){
    Mat imagemOriginal_gray;
    unsigned char *arrayImagemOriginal= new uchar [imagemOriginal.rows*imagemOriginal.cols];
    float *arrayImagemDestinoX= new float [imagemOriginal.rows*imagemOriginal.cols];
    float *arrayImagemDestinoY= new float [imagemOriginal.rows*imagemOriginal.cols];
    unsigned char *arrayImagemDestino= new uchar [imagemOriginal.rows*imagemOriginal.cols];
    /// Convert it to gray

    cvtColor( imagemOriginal, imagemOriginal_gray, CV_RGB2GRAY );
    imagemOriginal_gray.convertTo(imagemOriginal_gray, CV_8U);
    arrayImagemOriginal=(unsigned char*)imagemOriginal_gray.data;

    int nCol=imagemOriginal.cols;
    int nRow=imagemOriginal.rows;
     #pragma omp parallel for
    for(int i=1;i<imagemOriginal.rows-1;i++)
        for(int j=1; j<imagemOriginal.cols-1;j++){
            arrayImagemDestinoX[i*nCol+j] = arrayImagemOriginal[(i-1)*nCol+j]- arrayImagemOriginal[(i+1)*nCol+j];
            arrayImagemDestinoY[i*nCol+j] = arrayImagemOriginal[(i)*nCol+j-1]- arrayImagemOriginal[(i)*nCol+j+1];
        }

    float patch[p*p][2],saida[p*p][2],s[3];
    int  m=p*p,n=2;
 #pragma omp parallel for private(patch,s,saida)
    for(int i=p/2;i<nRow-p/2;i++){
        for(int j=p/2;j<nCol-p/2;j++){
            for(int ji=-p/2;ji<=p/2;ji++)
                for(int jj=-p/2;jj<=p/2;jj++){

                    int indicePatch,indiceImagem;
                    indicePatch=(ji+p/2)*p+(jj+p/2);
                    indiceImagem=(i+ji)*nCol+(j+jj);

                    //printf("patch => %d \n imagem => %d\n",indicePatch,indiceImagem);

                    patch[indicePatch][0]=arrayImagemDestinoX[indiceImagem];
                    patch[indicePatch][1]=arrayImagemDestinoY[indiceImagem];
                }
            dsvd(patch, m, n, s, saida);
            arrayImagemDestino[i*nCol+j]=(unsigned char) max(s[0],s[1]);
        }
    }

    Mat Resultado (imagemOriginal.rows, imagemOriginal.cols, DataType<unsigned char>::type,arrayImagemDestino);
    return Resultado;
}












#endif // GRADIENTES1_H
