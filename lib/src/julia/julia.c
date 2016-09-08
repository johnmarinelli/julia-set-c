#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "math/complex.h"
#include "png/png_write.h"
#include "julia/types.h"
#include "julia/julia.h"

//uint c = 0;

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
//    ++c;
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

// Critical section.
void insert_to_ary(bitmap_t* bitmap, uint x_offset, uint y_offset, double* rgba, pthread_mutex_t* mtx) {
  pthread_mutex_lock(mtx);
  color_t* px = bitmap->pixels + bitmap->width * y_offset + x_offset;

  px->r = rgba[0];
  px->g = rgba[1];
  px->b = rgba[2];
  pthread_mutex_unlock(mtx);
}

void julia(uint start_x, uint start_y, 
    uint end_x, uint end_y, 
    double rc, double ic, 
    uint total_width, uint total_height, 
    double zoom_amt, double x_off, double y_off,
    uint max_itrs,
    bitmap_t* bitmap,
    pthread_mutex_t* mtx) {
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
      
      int x_offset = x - start_x;
      int y_offset = y - start_y;

      insert_to_ary(bitmap, x_offset, y_offset, rgba, mtx);
    }
  }
}

/* main() for a thread */
void* thread_fn(void* vargs) {
  thread_fn_args* args = (thread_fn_args*) vargs;
  julia(args->start_x, args->start_y, 
      args->end_x, args->end_y, 
      args->rc, args->ic, 
      args->total_width, args->total_height, 
      args->zoom_amt, 
      args->x_off, args->y_off, 
      args->max_itrs, 
      args->bitmap,
      args->mtx);
}

void start(uint total_width, uint total_height, 
    double rc, double ic,
    double zoom_amt, 
    double x_off, double y_off,
    uint max_itrs) {
  bitmap_t pic;
  uint start_x = 0;
  uint start_y = 0;
  uint end_x = total_width;
  uint end_y = total_height;

  const uint NUM_THREADS = 4;

  // todo: implement map & reduce

  pthread_t t_hnd;
  pthread_mutex_t mtx;
  
  thread_fn_args thread_args;

  thread_args.mtx = &mtx;
  thread_args.start_x = start_x;
  thread_args.start_y = start_y;
  thread_args.end_x = end_x;
  thread_args.end_y = end_y;
  thread_args.total_width = total_width; 
  thread_args.total_height = total_height; 
  thread_args.rc = rc; 
  thread_args.ic = ic; 
  thread_args.zoom_amt = zoom_amt; 
  thread_args.x_off = x_off; 
  thread_args.y_off = y_off; 
  thread_args.max_itrs = max_itrs; 
  thread_args.bitmap = &pic;

  pic.width = total_width;
  pic.height = total_height;

  pic.pixels = calloc(pic.width * pic.height, sizeof(color_t));

  pthread_mutex_init(&mtx, NULL);
  pthread_create(&t_hnd, NULL, thread_fn, &thread_args);

  pthread_join(t_hnd, NULL);

//  printf("sq_poly_iteration called %d times.\n", c);

  save_png_to_file(&pic, "pic.png");
  free(pic.pixels);
}

