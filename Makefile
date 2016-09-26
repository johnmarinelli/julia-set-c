CC=g++
CFLAGS=-c -Wall
OPENGL_FLAGS_LMAO=-L/usr/local/lib -I/usr/local/include -framework OpenGL -lglfw3 -lglew

all: hello

hello: main.o application.o
    $(CC) application.o main.o -o juliaglsl -L/usr/local/lib -I/usr/local/include -framework OpenGL -lglfw3 -lglew 

main.o: main.cpp
    $(CC) $(CFLAGS) main.cpp

application.o: application.cpp
    $(CC) $(CFLAGS) application.cpp $(OPENGL_FLAGS_LMAO)

