#include <cstdlib>
#include "gtest/gtest.h"
#include "julia.h"

double get_value_from_3d_ary(double* arr, uint w, uint h, uint d, uint x, uint y, uint z) {
  return *(arr + (y * w + x) + (z * (w * h)));
}

void print_array(double* arr, uint w, uint h, uint d) {
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

const double err_margin = 1e-5;

/*
 * COMPLEX NUMBERS RELATED TESTS
 **/
 
TEST(complex_test, c_modulus_test)
{
  double exp = 12.0;
  double res = c_modulus(12.0, 0.0);
  EXPECT_NEAR(res, exp, err_margin);
}

TEST(complex_test, c_add_test)
{
  double exp[2] = { 4.0, 6.0 };
  double res[2] = {};
  c_add(1.0, 2.0, 3.0, 4.0, res);

  EXPECT_NEAR(res[0], exp[0], err_margin);
  EXPECT_NEAR(res[1], exp[1], err_margin);
}

TEST(complex_test, c_mult_test)
{
  double exp[2] = { -5.0, 10.0 };
  double res[2] = {};
  c_mult(1.0, 2.0, 3.0, 4.0, res);

  EXPECT_NEAR(res[0], exp[0], err_margin);
  EXPECT_NEAR(res[1], exp[1], err_margin);
}

/*
 * JULIA SET RELATED TESTS
 **/

TEST(julia_test, calculate_r_test)
{
  double exp = 2.85904;
  EXPECT_NEAR(calculate_r(4.0, 3.5), exp, err_margin);
}

TEST(julia_test, norm_coords_test)
{
  double exp[2] = { 3.96, 4.84 };
  double res[2] = {};
  norm_coords(9, 10, -1.5, 0.5, 0.5, 1.1, 0.6, 0.9, res);

  EXPECT_NEAR(res[0], exp[0], err_margin);
  EXPECT_NEAR(res[1], exp[1], err_margin);
}

TEST(julia_test, sq_poly_iteration_test)
{
  int exp = 10;
  EXPECT_EQ(sq_poly_iteration(0.15, 0.3, -0.7, 0.27015, 1.5, 300), exp);
}

TEST(julia_test, sq_poly_iteration_test_2)
{
  int exp = 300;
  EXPECT_EQ(sq_poly_iteration(0.0, 0.11, -0.7, 0.27015, 1.5, 300), exp);
}

TEST(julia_test, complex_heat_map_test)
{
  double exp[4] = { 255, 170, 85, 170 };
  double res[4] = {};
  complex_heat_map(200, 0, 300, 1.0, 1.5, res);

  EXPECT_NEAR(res[0], exp[0], err_margin);
  EXPECT_NEAR(res[1], exp[1], err_margin);
  EXPECT_NEAR(res[2], exp[2], err_margin);
  EXPECT_NEAR(res[3], exp[3], err_margin);
}
/*
TEST(julia_test, julia_test)
{
  const int width = 1;
  const int height = 1;
  const int total_width = 400;
  const int total_height = 400;
  const double zoom_amt = 1.1;
  const double x_off = 0.1;
  const double y_off = 0.1;
  const int max_itrs = 300;

  double exp[width][height][4] = { 
    {
      {255.0, 89.0, 166.0, 128.0}
    }
  };

  double* res;
  res = (double*)malloc(width * height * 4 * sizeof(double));

  julia(200, 200, 201, 201, -0.7, 0.27015, total_width, total_height, zoom_amt, x_off, y_off, max_itrs, res);

  EXPECT_NEAR(res[0], exp[0][0][0], err_margin);
  EXPECT_NEAR(res[1], exp[0][0][1], err_margin);
  EXPECT_NEAR(res[2], exp[0][0][2], err_margin);
  EXPECT_NEAR(res[3], exp[0][0][3], err_margin);

  free(res);
}*/

static color_t* pixel_at (bitmap_t * bitmap, int x, int y)
{
    return bitmap->pixels + bitmap->width * y + x;
}
    

TEST(julia_test, julia_test_2) 
{
  const int width = 4;
  const int height = 3;
  const int total_width = 400;
  const int total_height = 400;
  const double zoom_amt = 1.0;
  const double x_off = 0.0;
  const double y_off = 0.0;
  const int max_itrs = 300;
  const int total_size = width * height * 4;

  uint i,
       j,
       k;

  bitmap_t res;
  res.width = width;
  res.height = height;
  res.pixels = (color_t*) malloc(res.width * res.height * sizeof(color_t));

  double exp[width * height][4] = {
    {255.0, 81.00, 174.0, 128.0},
    {255.0, 81.00, 174.0, 128.0},
    {255.0, 78.00, 177.0, 128.0},
    {255.0, 76.00, 179.0, 128.0},
    {255.0, 82.00, 173.0, 128.0},
    {255.0, 82.00, 173.0, 128.0},
    {255.0, 144.0, 111.0, 128.0},
    {255.0, 255.0,  0.00, 128.0},
    {255.0, 83.00, 172.0, 128.0},
    {255.0, 85.00, 170.0, 128.0},
    {255.0, 255.0,  0.00, 128.0},
    {255.0, 156.0,  99.0, 128.0}
  };

  julia(200, 200, 200 + width, 200 + height, -0.7, 0.27016, total_width, total_height, zoom_amt, x_off, y_off, max_itrs, &res);

  for (i = 0; i < height; ++i) {
    for (j = 0; j < width; ++j) {
      color_t* col = pixel_at(&res, j,i);
      double rgba[4] = { col->r, col->g, col->b, col->a };

      uint k = 0;
      for (; k < 4; ++k) {
        double* f = exp[i * width + j];
        double e = *(f + k);
        EXPECT_NEAR(e, rgba[k], err_margin);
      }
    }
  }

  free(res.pixels);
}
