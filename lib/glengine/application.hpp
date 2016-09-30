#ifndef JOHN_APPLICATION_H
#define JOHN_APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "palettes.hpp" 
#include "shaders.hpp" 

namespace john
{

int compile_shaders(const char*, const char*);
bool write_png_file(char* filename, float* pixels, uint32_t width, uint32_t height);

class Application
{
public:
  Application();

  virtual ~Application() {}

  virtual void startup();

  virtual void run(john::Application* current_app);

  virtual void render(double currentTime);

  virtual void shutdown();

  virtual void onKey(int key, int action);

  virtual void on_mouse(int button, int action);

  virtual void handle_click();

  bool print_screen();

protected:
  static john::Application* app;

  static void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    app->onKey(key, action);
  }

  void get_mouse_position(int& x, int& y)
  {
      double dx, dy;
      glfwGetCursorPos(window, &dx, &dy);

      x = static_cast<int>(floor(dx));
      y = static_cast<int>(floor(dy));
  }

  static void glfw_on_mouse(GLFWwindow* window, int button, int action, int mods)
  {
    app->on_mouse(button, action);
  }

  GLuint compile_shaders(const char* vtx_shdr_src, const char* frg_shdr_src);

public:
  const uint32_t width = 1024;
  const uint32_t height = 768;

  GLuint programID;
  GLuint vao;
  GLuint palette_texture;

  GLFWwindow* window;

  /*
  struct {
    GLint   zoom;
    GLint   offset;
    GLint   C;
  } uniforms;

  bool paused;
  float time_offset;
  float zoom;
  float x_offset;
  float y_offset;
  */

};

}

#endif
