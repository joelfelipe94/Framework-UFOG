#ifndef SOFTMATTING_H
#define SOFTMATTING_H

#include "common.h"

void init_vars(int img_w, int img_h);
void loop(double *img, int *row_inds, int *col_inds, double *vals, int w, int h, double epsilon);
void free_vars();
Mat solve_alpha(const Mat *img_cv, const Mat *img_scr_cv);


#endif // SOFTMATTING_H
