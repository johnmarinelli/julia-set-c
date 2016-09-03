#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "png_write.h"
#include "julia.h"

// Calculate modulus of a complex number
// (its euclidean distance from origin)
double c_modulus(double r0, double i0) {
  return sqrt((r0 * r0) + (i0 * i0));
}

void c_add(double r0, double i0, double r1, double i1, double cpx[]) {
  cpx[0] = r0 + r1;
  cpx[1] = i0 + i1;
}

void c_mult(double r0, double i0, double r1, double i1, double cpx[]) {
  cpx[0] = (r0 * r1) - (i0 * i1);
  cpx[1] = (r0 * i1) + (i0 * r1);
}

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
  int i = 0;
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

void write_color_to_img(bitmap_t* img, double* rgba, uint x, uint y) {
  color_t* color = color_at(img, x, y);
  color->r = (int)rgba[0];
  color->g = (int)rgba[1];
  color->b = (int)rgba[2];
}

void julia(uint start_x, uint start_y, 
    uint end_x, uint end_y, 
    double rc, double ic, 
    uint total_width, uint total_height, 
    double zoom_amt, double x_off, double y_off,
    uint max_itrs,
    bitmap_t* img) {
  const int width = end_x - start_x;
  const int height = end_y - start_y;
  double radius = calculate_r(rc, ic);
  double r_min = radius * -1;
  double r_max = radius;
  double x_step = fabs(r_max - r_min) / (double)total_width;
  double y_step = fabs(r_max - r_min) / (double)total_height;
  double zmod = c_modulus(rc, ic);
  double new_coords[2] = {};
  double rgba[4] = {};
  int itrs = 0;

  int y = start_y,
      x;

  double* res[width * height * 4];

  for (; y < end_y; ++y) {
    for (x = start_x; x < end_x; ++x) {
      norm_coords(x, y, r_min, x_step, y_step, zoom_amt, x_off, y_off, new_coords);
      itrs = sq_poly_iteration(new_coords[0], new_coords[1], rc, ic, radius, max_itrs);
      complex_heat_map(itrs, 0, max_itrs, zmod, radius, rgba);
      write_color_to_img(img, rgba, x, y);

      //int colors_y_offset = y - start_y;
      //int colors_x_offset = x - start_x;
      //res[(width * colors_y_offset) + colors_x_offset] = rgba;
      //memcpy(&colors[(width * colors_y_offset) + colors_x_offset], &rgba, sizeof(rgba));
    }
  }
}
