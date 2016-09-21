#include "application.hpp"

int main(int argc, char* args[]) 
{
  john::Application* app = new john::Application;
  app->run(app);
  return 0;
}
