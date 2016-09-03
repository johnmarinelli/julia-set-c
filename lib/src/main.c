#include <stdio.h>
#include "png_write.h"
#include "julia.h"

static int pix(int value, int max) {
  if (value < 0) {
    return 0;
  }
  return (int) (256.0 * ((double) (value) / (double) max));
}

int main(int argc, char* args[]) {
  bitmap_t pic;
  int x, y;
  const uint width = 400,
        height = 400,
        total_width = 400,
        total_height = 400,
        start_x = 0,
        start_y = 0,
        end_x = start_x + width,
        end_y = start_y + height,
        max_itrs = 300;

  const double zoom_amt = 1.0,
        x_off = 0.0,
        y_off = 0.0,
        rc = -0.7,
        ic = 0.27015;

  pic.width = width;
  pic.height = height;

  pic.pixels = calloc(pic.width * pic.height, sizeof(color_t));
  if(!pic.pixels) { 
    return -1;
  }

  julia(start_x, start_y, end_x, end_y, rc, ic, total_width, total_height, zoom_amt, x_off, y_off, max_itrs, &pic);

  save_png_to_file(&pic, "pic.png");
  free(pic.pixels);
  return 0;
}
