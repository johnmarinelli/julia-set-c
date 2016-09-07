#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "math/complex.h"
#include "png/png_write.h"
#include "julia/types.h"
#include "julia/julia.h"

void norm_coords(uint x, uint y, 
    double r_min, 
    double x_step, double y_step, 
    double zoom_amt, 
    double x_off, double y_off, 
    double cpx[]) {
  cpx[0] = ((x * x_step) + x_off + r_min) * zoom_amt;
  cpx[1] = ((y * y_step) + y_off + r_min) * zoom_amt;
}

double calculate_r(double r0, double i0) {
  return (1 + sqrt(1 + (4 * c_modulus(r0, i0)))) / 2.0;
}

uint sq_poly_iteration(double r0, double i0, double rc, double ic, double radius, uint max_itrs) {
  uint i = 0;
  double last_zr = r0;
  double last_zi = i0;
  double dis = 0.0;

  while (i < max_itrs && dis < radius) {
    // This is the actual julia function
    double a[2] = {};
    c_mult(last_zr, last_zi, last_zr, last_zi, a); // z^2
    double b[2] = {};
    c_add(a[0], a[1], rc, ic, b); // z^2 + c
    last_zr = b[0];
    last_zi = b[1];
    dis = c_modulus(last_zr, last_zi);
    ++i;
  }

  return i;
}

void complex_heat_map(uint itrs, double min, double max, double zmod, double radius, double rgba[]) {
  double new_value = ((double)itrs - min) / (max - min);
  double zmod_div_radius = zmod / radius;
  rgba[0] = 255.0;
  rgba[1] = abs((int)roundl(255.0 * new_value) % 256);
  rgba[2] = abs((int)roundl(255.0 * (1.0 - new_value)) % 256);
  rgba[3] = abs((int)roundl(255.0 * (zmod_div_radius > 1.0 ? 1.0 : zmod_div_radius)) % 256);
}

/*
void write_color_to_img(bitmap_t* img, double* rgba, uint x, uint y) {
  color_t* color = color_at(img, x, y);
  color->r = (int)rgba[0];
  color->g = (int)rgba[1];
  color->b = (int)rgba[2];
}
*/

void julia(uint start_x, uint start_y, 
    uint end_x, uint end_y, 
    double rc, double ic, 
    uint total_width, uint total_height, 
    double zoom_amt, double x_off, double y_off,
    uint max_itrs,
    bitmap_t* bitmap) {
  double radius = calculate_r(rc, ic);
  double r_min = radius * -1;
  double r_max = radius;
  double x_step = fabs(r_max - r_min) / (double)total_width;
  double y_step = fabs(r_max - r_min) / (double)total_height;
  double zmod = c_modulus(rc, ic);
  double new_coords[2] = {};
  double rgba[4] = {};
  int itrs = 0;

  uint y = start_y,
      x;

  for (; y < end_y; ++y) {
    for (x = start_x; x < end_x; ++x) {
      norm_coords(x, y, r_min, x_step, y_step, zoom_amt, x_off, y_off, new_coords);
      itrs = sq_poly_iteration(new_coords[0], new_coords[1], rc, ic, radius, max_itrs);
      complex_heat_map(itrs, 0, max_itrs, zmod, radius, rgba);
      
      int y_offset = y - start_y;
      int x_offset = x - start_x;

      color_t* px = bitmap->pixels + bitmap->width * y_offset + x_offset;

      px->r = rgba[0];
      px->g = rgba[1];
      px->b = rgba[2];
    }
  }
}
