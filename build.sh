#!/usr/bin/env bash

g++ -c -Wall application.cpp -L/usr/local/lib -I/usr/local/include -I/usr/local/include/freetype2 -framework OpenGL -lglfw3 -lglew -lpng -lpngwriter\
  && g++ -c -Wall main.cpp -L/usr/local/lib -I/usr/local/include -framework OpenGL -lglfw3 -lglew \
  && g++ application.o main.o -L/usr/local/lib -I/usr/local/include -framework OpenGL -lglfw3 -lglew -lpng -lpngwriter -o juliaglsl \
  && ./juliaglsl 
