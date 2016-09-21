#!/usr/bin sh
g++ julia.cpp -o julia -L/usr/local/lib -I/usr/local/include -lglew -framework OpenGL -lglfw3
