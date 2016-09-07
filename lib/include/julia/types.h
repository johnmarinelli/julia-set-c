#ifndef JULIA_TYPES
#define JULIA_TYPES

#include <stdint.h>
#include <stddef.h>

// TODO: refactor include <pthread> out of this file
#include <pthread.h>

typedef unsigned int uint;

/* Colored pixel */
typedef struct {
    double r, g, b;
} color_t;

/* Picture */
typedef struct {
  color_t* pixels;
  size_t width,
         height;
} bitmap_t;

// Package to pass args from Thread 1 => Thread 2
typedef struct {
  pthread_mutex_t* mtx;
  int start_x;
  int start_y;
  int end_x;
  int end_y;
  double rc, ic;
  int total_width;
  int total_height;
  double zoom_amt;
  double x_off, y_off;
  int max_itrs;
  bitmap_t* bitmap;
} thread_fn_args;
#endif
