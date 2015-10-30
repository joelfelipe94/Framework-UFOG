#ifndef RESTORATION_H
#define RESTORATION_H

#include "softmatting.h"

class Restoration
{
public:
    Restoration();

    Mat restoreImageVeil(Mat Image,Mat transmission, Vec3f Veil);
    Mat restoreImageClassic(Mat Image,Mat transmission, Vec3f Veil);
    Mat refineTransmission(Mat Image, Mat transmission);
    Mat restoreImageRedChannel(Mat Image,Mat transmission, Vec3f Veil);
    Mat restoreImageChiang(Mat Image,Mat depth,Mat transmission, Vec3f Veil);
};

#endif // RESTORATION_H
