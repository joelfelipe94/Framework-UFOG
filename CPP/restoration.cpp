#include "restoration.h"

Restoration::Restoration()
{
}


Mat Restoration::restoreImageVeil(Mat Image,Mat transmission, Vec3f Veil)
{
    Mat finalImage;
    vector<Mat> channels,finalchannels;
    finalchannels.resize(3);
    split(Image,channels);


    Vec3f minTrans;


    minTrans[0] = 0.25;
    minTrans[1] = 0.25;
    minTrans[2] = 0.25;

    for (int i=0;i<3;i++){

        Mat B = Veil[i]*transmission;

        B = max(B,minTrans[i]);




        Mat A = channels[i] -Veil[i] + B;


        Mat C = A/(2*B);



        finalchannels[i] = max(min(C,1),0);
    }

    merge(finalchannels,finalImage);
    return finalImage;


}




Mat Restoration::restoreImageClassic(Mat Image,Mat transmission, Vec3f Veil)
{
    Mat finalImage;
    vector<Mat> channels,finalchannels;
    finalchannels.resize(3);
    split(Image,channels);

    transmission = max(transmission,0.10);

    transmission = min(transmission,0.95);

    for (int i=0;i<3;i++){
        Mat A = channels[i] -Veil[i];

        Mat B = transmission;

        Mat C = A/B;
        /// C is between -1 and 0
        C = max(-Veil[i],C);
        C = min(1-Veil[i],C);
        finalchannels[i] = C+Veil[i];
    }

    merge(finalchannels,finalImage);
    return finalImage;


}

Mat Restoration::restoreImageRedChannel(Mat Image,Mat transmission, Vec3f Veil)
{
    Mat finalImage;
    vector<Mat> channels,finalchannels;
    finalchannels.resize(3);
    split(Image,channels);

    transmission = max(transmission,0.10);

    transmission = min(transmission,0.95);
    Vec3f p;
    p[0]=1-Veil[0];
    p[1]=1-Veil[1];
    p[2]=1-Veil[2];
    for (int i=0;i<3;i++){
        Mat A = channels[i] -Veil[i];

        Mat B = transmission;
        Mat C = A/B;
        /// C is between -1 and 0
        C = max(-Veil[i],C);
        C = min(1-Veil[i],C);
        finalchannels[i] = C+Veil[i]*p[i];
    }

    merge(finalchannels,finalImage);
    return finalImage;


}


Mat Restoration::refineTransmission(Mat Image, Mat transmission)
{

    // Apply the conversions first.
    transmission = transmission*255;
    transmission.convertTo(transmission,CV_8U);

    cvtColor(transmission,transmission,CV_GRAY2BGR);


    cout << transmission.channels() << endl;
    Image = Image*255;
    cvtColor(Image, Image, CV_RGB2BGR);
    Image.convertTo(Image,CV_8UC3);
     //setNbThreads(omp_get_max_threads());
    //cout << transmission << endl;
     Mat img_cv = Image;
     Mat img_scr_cv = transmission;
     int img_w=img_cv.cols;
     int img_h=img_cv.rows;
     int img_scr_w=img_scr_cv.cols;
     int img_scr_h=img_scr_cv.rows;


     if(img_h!=img_scr_h || img_w!=img_scr_w){
         printf("As images tem tamanhos diferentes.\n");

     }

     init_vars(img_w, img_h);
     //while(...){

     Mat alpha_img=solve_alpha(&img_cv, &img_scr_cv);
     //}


     free_vars();



     return alpha_img;
}
