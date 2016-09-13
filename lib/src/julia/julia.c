#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "math/complex.h"
#include "math/linmath.h"
//#include "png/png_write.h"
#include "julia/types.h"
#include "julia/julia.h"

#include <unistd.h>

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
  double dis_sq = 0.0;
  double radius_sq = radius * radius;

  while (i < max_itrs && dis_sq < radius_sq) {
    // This is the actual julia function
    double a[2] = {};
    c_mult(last_zr, last_zi, last_zr, last_zi, a); // z^2
    double b[2] = {};
    c_add(a[0], a[1], rc, ic, b); // z^2 + c
    last_zr = b[0];
    last_zi = b[1];
    dis_sq = last_zr * last_zr + last_zi * last_zi;
//    ++c;
    ++i;
  }

  return i;
}

void complex_heat_map(uint itrs, double min, double max, double zmod, double radius, double rgba[]) {
  double new_value = ((double)itrs - min) / (max - min);
  double zmod_div_radius = zmod / radius;
  rgba[0] = 255.0;
  rgba[1] = (int)roundl(255.0 * new_value) % 256;
  rgba[2] = (int)roundl(255.0 * (1.0 - new_value)) % 256;
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

void julia(args_t* args, float* pixels) {
  double rc = args->rc;
  double ic = args->ic;
  uint32_t start_x = args->start_x;
  uint32_t start_y = args->start_y;

  double radius = calculate_r(rc, ic);
  double r_min = radius * -1;
  double r_max = radius;
  double x_step = fabs(r_max - r_min) / (double)args->total_width;
  double y_step = fabs(r_max - r_min) / (double)args->total_height;
  double zmod = c_modulus(rc, ic);
  double new_coords[2] = {};
  double rgba[3] = {};
  int itrs = 0;

  uint32_t y = args->start_y,
      x;
  args->zoom_amt -= 0.1;
  args->x_off += 0.01;

  uint32_t width = args->end_x - start_x;

  for (; y < args->end_y; ++y) {
    for (x = start_x; x < args->end_x; ++x) {
      norm_coords(x, y, r_min, x_step, y_step, args->zoom_amt, args->x_off, args->y_off, new_coords);
      itrs = sq_poly_iteration(new_coords[0], new_coords[1], rc, ic, radius, args->max_itrs);
      complex_heat_map(itrs, 0, args->max_itrs, zmod, radius, rgba);
      uint32_t x_offset = x - start_x,
               y_offset = y - start_y;
      uint32_t idx_clamp_3s = (y_offset * width + x_offset) * 3;
      pixels[idx_clamp_3s] = rgba[0] / 255.0;
      pixels[idx_clamp_3s + 1] = rgba[1] / 255.0;
      pixels[idx_clamp_3s + 2] = rgba[2] / 255.0;
    }
  }
}

/* main() for a thread 
void* thread_fn(void* vargs) {
  thread_fn_args* args = (thread_fn_args*) vargs;
  printf("Start: (%d, %d) | End: (%d, %d)\n", args->start_x, args->start_y, args->end_x, args->end_y);
  julia(args->start_x, args->start_y, 
      args->end_x, args->end_y, 
      args->rc, args->ic, 
      args->total_width, args->total_height, 
      args->zoom_amt, 
      args->x_off, args->y_off, 
      args->max_itrs, 
      args->pixels);
}

const uint NUM_THREADS = 4;

typedef struct j_threads {
  thread_fn_args args[NUM_THREADS];
  pthread_t t_hnds[NUM_THREADS];
} j_threads;

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

  uint i = 0;

  // initialize bitmap
  pic.width = total_width;
  pic.height = total_height;

  pic.pixels = calloc(pic.width * pic.height, sizeof(color_t));

  pthread_mutex_t mtx;
  pthread_mutex_init(&mtx, NULL);

  pthread_t t_hnd;

  thread_fn_args thread_args;

  thread_args.mtx = &mtx;
  thread_args.start_x = 0;
  thread_args.start_y = 0;
  thread_args.end_x = total_width;
  thread_args.end_y = total_height;
  thread_args.total_width = total_width; 
  thread_args.total_height = total_height; 
  thread_args.rc = rc; 
  thread_args.ic = ic; 
  thread_args.zoom_amt = zoom_amt; 
  thread_args.x_off = x_off; 
  thread_args.y_off = y_off; 
  thread_args.max_itrs = max_itrs; 
  thread_args.bitmap = &pic;

  j_threads threads;

  threads.args[0].start_x = 0;
  threads.args[0].start_y = 0;
  threads.args[0].end_x = total_width / 2;
  threads.args[0].end_y = total_height / 2;

  threads.args[1].start_x = total_width / 2;
  threads.args[1].start_y = 0;
  threads.args[1].end_x = total_width;
  threads.args[1].end_y = total_height / 2;
  
  threads.args[2].start_x = 0;
  threads.args[2].start_y = total_height / 2;
  threads.args[2].end_x = total_width / 2;
  threads.args[2].end_y = total_height;

  threads.args[3].start_x = total_width / 2;
  threads.args[3].start_y = total_height / 2;
  threads.args[3].end_x = total_width;
  threads.args[3].end_y = total_height;
  
  //thread_fn_args thread_args;
  for (i = 0; i < NUM_THREADS; ++i) {
    threads.args[i].mtx = &mtx;
    threads.args[i].total_width = total_width; 
    threads.args[i].total_height = total_height; 
    threads.args[i].rc = rc; 
    threads.args[i].ic = ic; 
    threads.args[i].zoom_amt = zoom_amt; 
    threads.args[i].x_off = x_off; 
    threads.args[i].y_off = y_off; 
    threads.args[i].max_itrs = max_itrs; 
    threads.args[i].bitmap = &pic;
  }

  for (i = 0; i < NUM_THREADS; ++i) {
    pthread_create(&threads.t_hnds[i], NULL, thread_fn, &threads.args[i]);
  }

  for (i = 0; i < NUM_THREADS; ++i) {
    int e = pthread_join(threads.t_hnds[i], NULL);
  }

  pthread_create(&t_hnd, NULL, thread_fn, &thread_args);
  pthread_join(t_hnd, NULL);

  save_png_to_file(&pic, "pic.png");
  free(pic.pixels);
}
  */

