#include "julia/julia.hpp"

int main(int argc, char* args[]) 
{
  julia::Julia* app = new julia::Julia;
  app->run(app);
  return 0;
}
