#include <iostream>
#include <fstream>

#include "julia/julia.hpp"

namespace julia 
{

julia::Julia::Julia() :
  john::Application(),
  paused(false),
  zoom(1.0f),
  x_offset(0.0f),
  y_offset(0.0f),
  time_offset(0.0f)
{
}

void julia::Julia::startup()
{
  john::Application::startup();

  programID = john::Application::compile_shaders(julia::shaders::vertex_shader_src, julia::shaders::fragment_shader_src);

  uniforms.zoom   = glGetUniformLocation(programID, "zoom");
  uniforms.offset = glGetUniformLocation(programID, "offset");
  uniforms.C      = glGetUniformLocation(programID, "C");

  // load the 1D palette texture 
  glGenTextures(1, &palette_texture);
  glBindTexture(GL_TEXTURE_1D, palette_texture);
  glTexStorage1D(GL_TEXTURE_1D, 8, GL_RGB8, 256);
  glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 256, GL_RGB, GL_UNSIGNED_BYTE, john::palettes::black_to_white);
  glGenerateMipmap(GL_TEXTURE_1D);
}

void julia::Julia::render(double current_time)
{
  static const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  glClearBufferfv(GL_COLOR, 0, color);

  static float t = 0.0f;
  float r = 0.0f;

  if (!paused)
    t = current_time;

  r = t;

  float C[2] = { (sinf(r * 0.1f) + cosf(r * 0.23f)) * 0.5f, (cosf(r * 0.13f) + sinf(r * 0.21f)) * 0.5f };
  float offset[2] = { x_offset, y_offset };

  glUseProgram(programID);

  glUniform2fv(uniforms.C, 1, C);
  glUniform2fv(uniforms.offset, 1, offset);
  glUniform1f(uniforms.zoom, zoom);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void julia::Julia::on_key(int key, int action)
{
  if (GLFW_PRESS == action) {
    switch(key) {
      case GLFW_KEY_SPACE: paused = !paused; 
                       break;
      case GLFW_KEY_P: print_screen();
                       break;
      default: break;
    }
  }
}

void julia::Julia::on_mouse(int button, int action) 
{
  if (GLFW_MOUSE_BUTTON_1 == button) {
    switch(action) {
      case GLFW_PRESS:
        handle_click();
        break;
      default: break;
    }
  }

}

void julia::Julia::handle_click() 
{
  int x, y;
  float pixels[3];
  get_mouse_position(x, y);

  int screen_stats[4];

  // get width/height of window
  glGetIntegerv(GL_VIEWPORT, screen_stats);

  glReadPixels(x, screen_stats[3] - y, 1, 1, GL_RGB, GL_FLOAT, pixels);
  printf("Value of pixel at (%d, %d): [%f %f %f]", x, y, pixels[0], pixels[1], pixels[2]);
}

void julia::Julia::shutdown()
{
}

}
