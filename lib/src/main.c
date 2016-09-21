#include <stdio.h>
#include "julia/julia.h"
#include "render/render.h"

int main(int argc, char* args[]) {
  int total_width = 250,
      total_height = 250,
      width = total_width,
      height = total_height,
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

  args_t input;
  input.total_width = total_width;
  input.total_height = total_height;
  input.width = width;
  input.height = height;
  input.start_x = start_x;
  input.start_y = start_y;
  input.end_x = end_x;
  input.end_y = end_y;
  input.max_itrs = max_itrs;
  input.zoom_amt = zoom_amt;
  input.x_off = x_off;
  input.y_off = y_off;
  input.rc = rc;
  input.ic = ic;

  const char* title = "Julia Set Explorer";
  float* pixels;

  GLFWwindow* window;
  int32_t status = 0;

  if (!glfwInit()) {
    status = -1;
  }

  window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (!window) {
    glfwTerminate();
    status = -1;
  }

  if (-1 != status) {
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
  }

  if (0 == status) {
    pixels = calloc(width * height * 3, sizeof(float));
    uint64_t ctr = 0;

    while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      julia(&input, pixels, ++ctr);

      glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    free(pixels);
    pixels = NULL;
  }

  cleanup_render(window);

  return 0;
}
