#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "types.h"
const double err_margin = 1e-5;

inline double get_value_from_3d_ary(double* arr, uint w, uint h, uint d, uint x, uint y, uint z) {
  return *(arr + (y * w + x) + (z * (w * h)));
}

void print_3d_ary(double* arr, uint w, uint h, uint d) {
  uint i,
       j,
       k;

  for (k = 0; k < d; ++k) {
    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        printf("i: %d j: %d k: %d | ", i, j, k);
        printf("%f\n", get_value_from_3d_ary(arr, w, h, d, j, i, k));
      }
      printf("\n");
    }
  }
}

static color_t* pixel_at (bitmap_t * bitmap, int x, int y)
{
    return bitmap->pixels + bitmap->width * y + x;
}

#endif
