#include <pngwriter.h>
#include "glengine/application.hpp"
#include "common/utilities.hpp"

#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

john::Application* john::Application::app = NULL;

namespace john 
{


john::Application::Application() :
  width(0),
  height(0)
{
}

void john::Application::startup() 
{
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
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
  
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();

  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  width = mode->width;
  height = mode->height;

  window = glfwCreateWindow(width, height, "My Title", monitor, NULL);
  //window = glfwCreateWindow(width, height, "Julia", glfwGetPrimaryMonitor(), NULL);
  if( window == NULL ){
    fprintf( stderr, "Failed to open GLFW window.\n" );
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, glfw_onKey);
  //glfwSetMouseButtonCallback(window, glfw_on_mouse);

  /*glfwSetWindowSizeCallback(window, glfw_onResize);
  glfwSetCursorPosCallback(window, glfw_onMouseMove);*/
  glfwSetScrollCallback(window, glfw_on_mouse_wheel);

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

  std::time_t t = std::time(NULL);
  std::tm tm = *std::localtime(&t);
  std::ostringstream oss;
  oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
  std::string time_str = oss.str();
  std::string ext = std::string(".png");
  std::string filename = time_str + ext;

  john::utilities::write_png_file(filename.c_str(), pixels, width, height);
}

GLuint john::Application::compile_shaders(const char* vtx_shdr_src, const char* frg_shdr_src) 
{
  return john::utilities::compile_shaders(vtx_shdr_src, frg_shdr_src);
}


void john::Application::shutdown() 
{
}

}
