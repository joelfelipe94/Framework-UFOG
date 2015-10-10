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

    transmission = max(transmission,0.25);

    transmission = min(transmission,0.95);

    for (int i=0;i<3;i++){

        Mat B = Veil[i]*transmission;
        Mat A = channels[i] -Veil[i] + B;

        A = max(0,A);
        A = min(1,A);

        Mat C = A/(2*B);



        finalchannels[i] = C;
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

    transmission = max(transmission,0.25);

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
