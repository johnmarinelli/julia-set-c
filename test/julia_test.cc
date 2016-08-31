#include "gtest/gtest.h"
#include "julia.h"

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
