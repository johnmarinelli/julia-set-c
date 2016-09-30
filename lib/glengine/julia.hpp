#ifndef JULIA_SET_H
#define JULIA_SET_H

#include "glengine/application.hpp"

namespace julia 
{

class Julia : protected john::Application
{
public:
  Julia();

  void startup();

  void render(double current_time);

  void shutdown();

  void on_key(int key, int action);

  void on_mouse(int button, int action);

  void handle_click();

private:
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
}

}

#endif
