#include <stdio.h>
#include "julia/julia.h"
#include "render/render.h"

int32_t init_render(GLFWwindow* window, uint32_t width, uint32_t height, const char* title) {
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

  return status;
}

int main(int argc, char* args[]) {
  printf("hello");
  int total_width = 100,//atoi(args[1]),
      total_height = 100,//atoi(args[2]),
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

    while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      julia(start_x, start_y, 
          end_x, end_y,
          rc, ic,
          width, height,
          zoom_amt += -0.01, x_off, y_off,
          max_itrs,
          pixels);

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
