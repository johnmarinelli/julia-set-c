#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include "julia/julia.h"

int main(int argc, char* args[]) {
  int total_width = atoi(args[1]),
      total_height = atoi(args[2]),
      width = total_width,
      height = total_height,
      start_x = 0,
      start_y = 0,
      end_x = start_x + width,
      end_y = start_y + height,
      max_itrs = 300;

  double zoom_amt = 0.1,
        x_off = 0.25,
        y_off = 0.5,
        rc = -0.7,
        ic = 0.27015;

  start(total_width, total_height, rc, ic, zoom_amt, x_off, y_off, max_itrs);
  return 0;
}
