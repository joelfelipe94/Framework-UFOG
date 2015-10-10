#ifndef RESTORATION_H
#define RESTORATION_H

#include "common.h"

class Restoration
{
public:
    Restoration();
    Mat restoreImageVeil(Mat Image,Mat transmission, Vec3f Veil);
    Mat restoreImageClassic(Mat Image,Mat transmission, Vec3f Veil);
};

#endif // RESTORATION_H
