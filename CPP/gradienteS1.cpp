#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include<time.h>
#include "gradienteS1.h"
#define SIGN(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

using namespace cv;



int main( int argc, char** argv )
{
    clock_t start = clock();

    Mat imagemOriginal,Resultado;
    char* window_name = "Resultado";


    /// Load an image
    imagemOriginal = imread( argv[1] );

    Resultado=gradienteS1(imagemOriginal,9);
     clock_t end = clock();
    printf("tempo=>  %lf \n", double(end - start) / CLOCKS_PER_SEC);
    /// Create window
    namedWindow( "Resultado", CV_WINDOW_AUTOSIZE );
    imshow( "Resultado", Resultado);

    waitKey(0);

    return 0;
}
