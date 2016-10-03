#ifndef JOHN_APPLICATION_H
#define JOHN_APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <queue>

#include "julia/palettes.hpp" 
#include "julia/shaders.hpp" 

namespace john
{

class Application
{
public:
  Application();

  virtual ~Application() {}

  virtual void startup();

  virtual void run(john::Application* current_app);

  virtual void render(double currentTime) = 0;

  virtual void shutdown();

  bool print_screen();

protected:
  static john::Application* app;

  static void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    app->on_key(key, action);
  }

  static void glfw_on_mouse_wheel(GLFWwindow* window, double xoffset, double yoffset)
  {
    app->on_scroll(xoffset, yoffset);
  }

  void get_mouse_position(int& x, int& y)
  {
      double dx, dy;
      glfwGetCursorPos(window, &dx, &dy);

      x = static_cast<int>(floor(dx));
      y = static_cast<int>(floor(dy));
  }

  virtual void on_key(int key, int action) = 0;
  virtual void on_scroll(double x_scroll_offset, double y_scroll_offset) = 0;

  GLuint compile_shaders(const char* vtx_shdr_src, const char* frg_shdr_src);

public:
  uint32_t width;
  uint32_t height;

  GLuint programID;
  GLuint vao;
  GLuint palette_texture;
  std::queue<GLuint> palette_textures;

  GLFWwindow* window;
};

}

#endif
