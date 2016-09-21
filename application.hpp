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
int compile_shaders(const char* vtx_shdr_txt, const char* frg_shdr_txt) {
  GLuint vtx_shader, frg_shader, programID;
  vtx_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vtx_shader, 1,&vtx_shdr_txt, NULL);
  glCompileShader(vtx_shader);

  frg_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frg_shader, 1, &frg_shdr_txt, NULL);
  glCompileShader(frg_shader);

  programID = glCreateProgram();
  glAttachShader(programID, vtx_shader);
  glAttachShader(programID, frg_shader);
  glLinkProgram(programID);

  glDeleteShader(vtx_shader);
  glDeleteShader(frg_shader);

  return programID;
}


class Application
{
public:
  Application() :
    paused(false),
    zoom(1.0f),
    x_offset(0.0f),
    y_offset(0.0f),
    time_offset(0.0f)
  {
  }

  virtual ~Application() {}

  virtual void run(john::Application* app)
  {
    bool running = true;

    // Initialise GLFW
    if( !glfwInit() ) {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      return;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(1024, 768, "Julia", NULL, NULL);
    if( window == NULL ){
      fprintf( stderr, "Failed to open GLFW window.\n" );
      glfwTerminate();
      return;
    }

    glfwMakeContextCurrent(window);
/*
        glfwSetWindowSizeCallback(window, glfw_onResize);
        glfwSetKeyCallback(window, glfw_onKey);
        glfwSetMouseButtonCallback(window, glfw_onMouseButton);
        glfwSetCursorPosCallback(window, glfw_onMouseMove);
        glfwSetScrollCallback(window, glfw_onMouseWheel);*/

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to initialize GLEW\n");
      glfwTerminate();
      return;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    startup();

    do {
      render(glfwGetTime());

      glfwSwapBuffers(window);
      glfwPollEvents();

      running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
      running &= (glfwWindowShouldClose(window) != GL_TRUE);
    } while (running);

    shutdown(); 
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  virtual void startup()
  {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* load and set the julia shader */
    programID = compile_shaders(john::shaders::vertex_shader_src, john::shaders::fragment_shader_src);
    uniforms.zoom   = glGetUniformLocation(programID, "zoom");
    uniforms.offset = glGetUniformLocation(programID, "offset");
    uniforms.C      = glGetUniformLocation(programID, "C");
    /* load the 1D palette texture */
    glGenTextures(1, &palette_texture);
    glBindTexture(GL_TEXTURE_1D, palette_texture);
    glTexStorage1D(GL_TEXTURE_1D, 8, GL_RGB8, 256);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 256, GL_RGB, GL_UNSIGNED_BYTE, john::palettes::orange);
    glGenerateMipmap(GL_TEXTURE_1D);
  }

  virtual void render(double currentTime) 
  {
    // Clear the screen
    static const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, color);

    static float t = 0.0f;
    float r = 0.0f;

    if (!paused)
      t = currentTime;

    r = t + time_offset;

    float C[2] = { (sinf(r * 0.1f) + cosf(r * 0.23f)) * 0.5f, (cosf(r * 0.13f) + sinf(r * 0.21f)) * 0.5f };
    float offset[2] = { x_offset, y_offset };

    glUseProgram(programID);

    glUniform2fv(uniforms.C, 1, C);
    glUniform2fv(uniforms.offset, 1, offset);
    glUniform1f(uniforms.zoom, zoom);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  }

  virtual void shutdown()
  {

  }


  struct {
    GLint   zoom;
    GLint   offset;
    GLint   C;
  } uniforms;

  GLuint programID;
  GLuint vao;
  GLuint palette_texture;

  bool paused;
  float time_offset;
  float zoom;
  float x_offset;
  float y_offset;
  GLFWwindow* window;
};

}

#endif
