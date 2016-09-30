CC=g++
CFLAGS=-c -Wall
MAC_BULLSHIT=-L/usr/local/lib -I/usr/local/include 
OPENGL_FLAGS_LMAO=-framework OpenGL -lglfw3 -lglew
PNGWRITER_LINK=-lpngwriter
PNGWRITER_FREETYPE_DEPENDENCY_LOCATION=-L/usr/local/include/freetype2 

all: main.o application.o
	$(CC) application.o main.o -o juliaglsl $(MAC_BULLSHIT) $(PNGWRITER_FREETYPE_DEPENDENCY_LOCATION) $(OPENGL_FLAGS_LMAO) $(PNGWRITER_LINK)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

application.o: application.cpp
	$(CC) $(CFLAGS) application.cpp $(MAC_BULLSHIT) $(PNGWRITER_FREETYPE_DEPENDENCY_LOCATION) $(OPENGL_FLAGS_LMAO) $(PNGWRITER_LINK)

clean:
	rm juliaglsl *.o *.png *.bmp *.jpg
