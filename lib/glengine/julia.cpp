#include <pngwriter.h>
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

}
