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


    minTrans[0] = 0.1;
    minTrans[1] = 0.1;
    minTrans[2] = 0.1;
    for (int i=0;i<3;i++){

        Mat I_lambda= channels[i];
        double A_lambda= Veil[i];
        Mat t= transmission;
        Mat A_lambdat=A_lambda*t;
        double t0=minTrans[i];
        Mat M;
        M=max((I_lambda-A_lambda+A_lambdat),0)/(max(t0,A_lambdat));

//        Mat B = Veil[i]*transmission;

//        B = max(B,minTrans[i]);

//        Mat A = channels[i] -Veil[i] + B;
//        int temp;
        double mini, maxi;
//        double miniLoc,maxiLoc;

//        Mat C = A/B; // se o maximo dessa operação passar de 1 ela deve ser normalizada para um



        minMaxLoc(M, &mini, &maxi);
//        cout<<endl<<"*****"<<mini<<" "<<maxi;
//        C=(C-mini)/(maxi-mini);
        cout<<maxi<<" "<<mini<<endl;
        if((maxi)>1)
            M=M/maxi;

        finalchannels[i] = max(min(M,1),0);
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
