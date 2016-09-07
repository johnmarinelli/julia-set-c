#include <stdio.h>
#include "julia/julia.h"

int main(int argc, char* args[]) {
  int width = 1000,
        height = 1000,
        total_width = 1000,
        total_height = 1000,
        start_x = 0,
        start_y = 0,
        end_x = start_x + width,
        end_y = start_y + height,
        max_itrs = 300;

  double zoom_amt = 1.0,
        x_off = 0.0,
        y_off = 0.0,
        rc = -0.7,
        ic = 0.27015;

  start(total_width, total_height, rc, ic, zoom_amt, x_off, y_off, max_itrs);
  return 0;
}
