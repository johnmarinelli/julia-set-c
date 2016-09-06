#include "timer.h"

int main(int argc, char* args[])
{
  double t1 = get_time();

  printf("t1: %f\n", t1);

  double t2 = get_time();

  printf("t2: %f\n", t2);

  return 0;
}

