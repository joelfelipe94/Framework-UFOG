#include "common.h"



#define MAX(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })


#define MIN(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })



#define rowPtr(imagePtr, dataType, lineIndex) \
        (dataType *)(imagePtr->imageData + (lineIndex) * imagePtr->widthStep)

void fft(Mat I, Mat & amp, Mat & phase){
    //BORDER_CONSTANT
    // Mat is already a grayscale image ready to reaceive a dft transfomation

    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values
    //copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
    /// This image has a the original I image but with borders  ( called here as padded )
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32FC1)};
    Mat complexI;
    complexI.convertTo(complexI,CV_32FC2);

    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros


    dft(complexI, complexI,DFT_SCALE);            // this way the result may fit in the source matrix
    cout << " Complex channels " << complexI.channels() << endl;
    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))


    Mat im = planes[1];
    Mat re = planes[0];
    phase = Mat::zeros(im.size(), CV_32FC1);
    //cout << "PHASE " << endl;
    float *input = ( float*)(phase.data);

    cout << phase.cols << endl;
    cout << phase.rows << endl;

    for(int i=0;i<phase.rows;i++){
        for(int j=0;j<phase.cols;j++){

            input[i*phase.cols + j]=atan2(im.at<float>(i,j),re.at<float>(i,j));
            //input[i*phase.cols +j] = 1;
        }
    }
    // planes[0] = magnitude
    magnitude(re,im,amp);


    /*amp += Scalar::all(1);                    // switch to logarithmic scale
    log(amp, amp);
    amp = amp(Rect(0, 0, amp.cols & -2, amp.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = amp.cols/2;
    int cy = amp.rows/2;

    Mat q0(amp, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(amp, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(amp, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(amp, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);



    // crop the spectrum, if it has an odd number of rows or columns analyse the necessity
    amp = amp(Rect(0, 0, amp.cols & -2, amp.rows & -2));*/
    //normalize(amp, amp, 0, 1, NORM_MINMAX);

 }

/// This is made for just grayscale image

Mat computeHistogram(Mat image)
{
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat hist;

     calcHist( &image, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );
    normalize(hist, hist, 0, histSize, NORM_MINMAX, -1, Mat() );
    return hist;
}


void drawHist(Mat hist)
{
    int histSize = 256;
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );


    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );


    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
        //line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
        //                 Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
        //                 Scalar( 0, 255, 0), 2, 8, 0  );
        //line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
        //                 Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
        //                 Scalar( 0, 0, 255), 2, 8, 0  );
    }


    imwrite("hist.jpg",histImage);

}
