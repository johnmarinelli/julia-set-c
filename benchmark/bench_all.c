#include <float.h> // DBL_MAX, DBL_MIN

#include "timer.h"
#include "julia/julia.h"

// Number of times to benchmark a function.
// Benchmark is taken by running function NUM_ITRS times
// and taking the average benchmark.
const uint NUM_ITRS = 10000;

void bench_c_modulus() {
  uint i = 0;
  double total = 0,
         max = DBL_MIN,
         min = DBL_MAX;

  for (; i < NUM_ITRS; ++i) {
    double t1 = get_time();
    c_modulus(0.7, -0.27015);
    double elapsed = get_time() - t1;
    total += elapsed;

    max = (elapsed > max) ? elapsed : max;
    min = (elapsed < min) ? elapsed : min;
  }

  double avg = total / (double) NUM_ITRS;
  printf("%s | %f | %f | %f\n", "c_modulus", avg, max, min);
}

void bench_c_add() {
  uint i = 0;
  double total = 0,
         max = DBL_MIN,
         min = DBL_MAX;

  for (; i < NUM_ITRS; ++i) {
    double res[2] = {};
    double t1 = get_time();
    c_add(0.7, -0.27015, 0.08, 0.27016, res);
    double elapsed = get_time() - t1;
    total += elapsed;

    max = (elapsed > max) ? elapsed : max;
    min = (elapsed < min) ? elapsed : min;
  }

  double avg = total / (double) NUM_ITRS;
  printf("%s | %f | %f | %f\n", "c_add", avg, max, min);
}

void bench_c_mult() {
  uint i = 0;
  double total = 0,
         max = DBL_MIN,
         min = DBL_MAX;

  for (; i < NUM_ITRS; ++i) {
    double res[2] = {};
    double t1 = get_time();
    c_mult(0.7, -0.27015, 1.18, 2.016, res);
    double elapsed = get_time() - t1;
    total += elapsed;

    max = (elapsed > max) ? elapsed : max;
    min = (elapsed < min) ? elapsed : min;
  }

  double avg = total / (double) NUM_ITRS;
  printf("%s | %f | %f | %f\n", "c_mult", avg, max, min);
}

void bench_norm_coords() {
  uint i = 0;
  double total = 0,
         max = DBL_MIN,
         min = DBL_MAX;

  for (; i < NUM_ITRS; ++i) {
    double res[2] = {};
    double t1 = get_time();
    norm_coords(25, 32, -1.5, 0.015, 0.015, 1.1, 0.18, 2.016, res);
    double elapsed = get_time() - t1;
    total += elapsed;

    max = (elapsed > max) ? elapsed : max;
    min = (elapsed < min) ? elapsed : min;
  }

  double avg = total / (double) NUM_ITRS;
  printf("%s | %f | %f | %f\n", "norm_coords", avg, max, min);
}

void bench_sq_poly_iteration() {
  uint i = 0;
  double total = 0,
         max = DBL_MIN,
         min = DBL_MAX;
  uint itrs;

  for (; i < NUM_ITRS; ++i) {
    double res[2] = {};
    double t1 = get_time();
    itrs = sq_poly_iteration(0.75, 0.75, 0.7, -0.27015, 1.5, 300);
    double elapsed = get_time() - t1;
    total += elapsed;

    max = (elapsed > max) ? elapsed : max;
    min = (elapsed < min) ? elapsed : min;
  }

  double avg = total / (double) NUM_ITRS;
  printf("%s (%d itrs) | %f | %f | %f\n", "sq_poly_iteration", itrs, avg, max, min);
}

void bench_julia() {
  uint i = 0;
  double total = 0,
         max = DBL_MIN,
         min = DBL_MAX;
  uint itrs,
       width = 50,
       height = 50;

  for (; i < NUM_ITRS; ++i) {
    bitmap_t res;
    res.width = width;
    res.height = height;
    res.pixels = (color_t*) malloc(res.width * res.height * sizeof(color_t));
    double t1 = get_time();
    julia(0, 0, width, height, -0.7, 0.27015, 400, 400, 1.0, 0.0, 0.0, 300, &res);
    double elapsed = get_time() - t1;
    free(res.pixels);
    total += elapsed;

    max = (elapsed > max) ? elapsed : max;
    min = (elapsed < min) ? elapsed : min;
  }

  double avg = total / (double) NUM_ITRS;
  printf("%s | %f | %f | %f\n", "julia", avg, max, min);
}


int main(int argc, char* args[])
{
  printf("Function | Average | Max | Min\n");
  bench_c_modulus();
  bench_c_add();
  bench_c_mult();
  bench_norm_coords();
  bench_sq_poly_iteration();
  bench_julia();
  return 0;
}

