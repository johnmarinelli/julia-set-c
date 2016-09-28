#include <pngwriter.h>
#include "application.hpp"

#include <iostream>
#include <fstream>

john::Application* john::Application::app = NULL;

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

bool write_png_file(char* filename, float* pixels, uint32_t width, uint32_t height) 
{
  uint32_t x, y, ctr;
  pngwriter png(width, height, 0, filename);
  printf("Width: %d Height: %d\n", width, height);

  ctr = 0;

  for (y = 0; y < height; ++y) { 
    for (x = 0; x < width; ++x) { 
      float r, g, b;
      uint32_t y_off = y * width;
      uint32_t off = y > 0 ? x * 3 - y :  x * 3;

      r = pixels[ctr];
      g = pixels[ctr + 1];
      b = pixels[ctr + 2];

      png.plot(x, y, r, g, b);
      ctr += 3;
    }
  }

  png.close();
  printf("Finished writing png.\n");
  return true;
}

john::Application::Application() :
    paused(false),
    zoom(1.0f),
    x_offset(0.0f),
    y_offset(0.0f),
    time_offset(0.0f)
{
}

void john::Application::startup() 
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
  glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 256, GL_RGB, GL_UNSIGNED_BYTE, john::palettes::black_to_white);
  glGenerateMipmap(GL_TEXTURE_1D);
}

void john::Application::run(john::Application* current_app) 
{
  app = current_app;
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

  window = glfwCreateWindow(width, height, "Julia", NULL, NULL);
  if( window == NULL ){
    fprintf( stderr, "Failed to open GLFW window.\n" );
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, glfw_onKey);
  glfwSetMouseButtonCallback(window, glfw_on_mouse);

  /*glfwSetWindowSizeCallback(window, glfw_onResize);
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

void john::Application::render(double currentTime) 
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

void john::Application::onKey(int key, int action) 
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

void john::Application::on_mouse(int button, int action) 
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

void john::Application::handle_click() 
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

bool john::Application::print_screen()
{
  float* pixels;
  int screen_stats[4];

  // get width/height of window
  glGetIntegerv(GL_VIEWPORT, screen_stats);

  uint32_t width = screen_stats[2];
  uint32_t height = screen_stats[3];
  pixels = new float[width * height * 3];
  glReadPixels(
    0, 
    0, 
    width,
    height,
    GL_RGB, 
    GL_FLOAT, 
    pixels
  );

  write_png_file("out.png", pixels, width, height);
}

void john::Application::shutdown() 
{
}

}
