#ifndef JULIA_SET_H
#define JULIA_SET_H

#include "glengine/application.hpp"

namespace julia 
{

class Julia : public john::Application
{
public:
  Julia();

  void startup();

  void render(double current_time);

  void shutdown();

  void on_mouse(int button, int action);

  void handle_click();

protected:
  void on_key(int key, int action);

  void on_scroll(double x_scroll_offset, double y_scroll_offset);

  void navigate(int key);

  void control_speed(int key);

  void cycle_palette();

private:
  struct {
    GLfloat   zoom;
    GLint   offset;
    GLint   C;
  } uniforms;

  bool paused;
  float zoom;
  float x_offset;
  float y_offset;
  float time_offset;

  GLuint palette_texture2;

};

}

#endif
