#include "gradients1prior.h"


double GradientS1Prior::PYTHAG(double a, double b)
{
    double at = fabs(a), bt = fabs(b), ct, result;
    if (at > bt)       { ct = bt / at; result = at * sqrt(1.0 + ct * ct); }
    else if (bt > 0.0) { ct = at / bt; result = bt * sqrt(1.0 + ct * ct); }
    else result = 0.0;
    return(result);
}

int GradientS1Prior::dsvd(double a[][2], int m, int n, double w[], double v[][2])
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
                    a[k][i] = (double)((double)a[k][i]/scale);    //aplica normalização
                    s += ((double)a[k][i] * (double)a[k][i]);    //quadrado do valor singular
                }
                f = (double)a[i][i];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                a[i][i] = (double)(f - g);
                if (i != n - 1)
                {
                    for (j = l; j < n; j++)
                    {
                        for (s = 0.0, k = i; k < m; k++)
                            s += ((double)a[k][i] * (double)a[k][j]);
                        f = s / h;
                        for (k = i; k < m; k++)
                            a[k][j] += (double)(f * (double)a[k][i]);
                    }
                }
                for (k = i; k < m; k++)
                    a[k][i] = (double)((double)a[k][i]*scale);
            }
        }
        w[i] = (double)(scale * g);

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
                    a[i][k] = (double)((double)a[i][k]/scale);
                    s += ((double)a[i][k] * (double)a[i][k]);
                }
                f = (double)a[i][l];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                a[i][l] = (double)(f - g);
                for (k = l; k < n; k++)
                    rv1[k] = (double)a[i][k] / h;
                if (i != m - 1)
                {
                    for (j = l; j < m; j++)
                    {
                        for (s = 0.0, k = l; k < n; k++)
                            s += ((double)a[j][k] * (double)a[i][k]);
                        for (k = l; k < n; k++)
                            a[j][k] += (double)(s * rv1[k]);
                    }
                }
                for (k = l; k < n; k++)
                    a[i][k] = (double)((double)a[i][k]*scale);
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
                    v[j][i] = (double)(((double)a[i][j] / (double)a[i][l]) / g);
                /* double division to avoid underflow */
                for (j = l; j < n; j++)
                {
                    for (s = 0.0, k = l; k < n; k++)
                        s += ((double)a[i][k] * (double)v[k][j]);
                    for (k = l; k < n; k++)
                        v[k][j] += (double)(s * (double)v[k][i]);
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
                        a[k][j] += (double)(f * (double)a[k][i]);
                }
            }
            for (j = i; j < m; j++)
                a[j][i] = (double)((double)a[j][i]*g);
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
                        w[i] = (double)h;
                        h = 1.0 / h;
                        c = g * h;
                        s = (- f * h);
                        for (j = 0; j < m; j++)
                        {
                            y = (double)a[j][nm];
                            z = (double)a[j][i];
                            a[j][nm] = (double)(y * c + z * s);
                            a[j][i] = (double)(z * c - y * s);
                        }
                    }
                }
            }
            z = (double)w[k];
            if (l == k)
            {                  /* convergence */
                if (z < 0.0)
                {              /* make singular value nonnegative */
                    w[k] = (double)(-z);
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
                    v[jj][j] = (double)(x * c + z * s);
                    v[jj][i] = (double)(z * c - x * s);
                }
                z = PYTHAG(f, h);
                w[j] = (double)z;
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
                    a[jj][j] = (double)(y * c + z * s);
                    a[jj][i] = (double)(z * c - y * s);
                }
            }
            rv1[l] = 0.0;
            rv1[k] = f;
            w[k] = (double)x;
        }
    }
    free((void*) rv1);
    return(1);
}

Mat GradientS1Prior::computePrior(Mat image, int patchsize)
{
    Mat imagemOriginal_gray;
    double *arrayImagemOriginal= new double [image.rows*image.cols];
    double *arrayImagemDestinoX= new double [image.rows*image.cols];
    double *arrayImagemDestinoY= new double [image.rows*image.cols];
    double *arrayImagemDestino= new double [image.rows*image.cols];
    /// Convert it to gray

    cvtColor( image, imagemOriginal_gray, CV_RGB2GRAY );
    imagemOriginal_gray.convertTo(imagemOriginal_gray, CV_64F);
    arrayImagemOriginal=( double* )imagemOriginal_gray.data;
    int nCol=image.cols;
    int nRow=image.rows;
     #pragma omp parallel for
    for(int i=1;i<image.rows-1;i++)
        for(int j=1; j<image.cols-1;j++){
            arrayImagemDestinoX[i*nCol+j] = arrayImagemOriginal[(i-1)*nCol+j]- arrayImagemOriginal[(i+1)*nCol+j];
            arrayImagemDestinoY[i*nCol+j] = arrayImagemOriginal[(i)*nCol+j-1]- arrayImagemOriginal[(i)*nCol+j+1];
        }
    double patch[patchsize*patchsize][2],saida[patchsize*patchsize][2],s[3];
    int  m=patchsize*patchsize,n=2;
 #pragma omp parallel for private(patch,s,saida)
    for(int i=patchsize/2;i<nRow-patchsize/2;i++){
        for(int j=patchsize/2;j<nCol-patchsize/2;j++){
            for(int ji=-patchsize/2;ji<=patchsize/2;ji++)
                for(int jj=-patchsize/2;jj<=patchsize/2;jj++){

                    int indicePatch,indiceImagem;
                    indicePatch=(ji+patchsize/2)*patchsize+(jj+patchsize/2);
                    indiceImagem=(i+ji)*nCol+(j+jj);

                    //printf("patch => %d \n imagem => %d\n",indicePatch,indiceImagem);
                    patch[indicePatch][0]=arrayImagemDestinoX[indiceImagem];
                    patch[indicePatch][1]=arrayImagemDestinoY[indiceImagem];
                }
            dsvd(patch, m, n, s, saida);
            arrayImagemDestino[i*nCol+j]=(double) max(s[0],s[1]);

        }
    }

    Mat Resultado (image.rows, image.cols,CV_64F,arrayImagemDestino);
    return Resultado;
}

Mat GradientS1Prior::computeTransmission(Mat image, int patchsize)
{
    return computePrior(image,patchsize); //TODO fazer o soft-matting
}
