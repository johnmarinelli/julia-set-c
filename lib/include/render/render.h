#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include <GLFW/glfw3.h>

void cleanup_render(GLFWwindow* window) {
  glfwDestroyWindow(window);
  window = NULL;
  glfwTerminate();
}

#endif
