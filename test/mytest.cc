#include "gtest/gtest.h"
#include "my_fn.h"
 
TEST(firstTest, abs)
{
  EXPECT_EQ(1, abs( -1 ));
  EXPECT_EQ(1, abs( 1 ));
}

TEST(secondTest, abs)
{
  EXPECT_EQ(my_func(), -1);
}
