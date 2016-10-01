CC=g++
CFLAGS=-c -Wall
MAC_BULLSHIT_INCLUDE=-I/usr/local/include 
MAC_BULLSHIT_LIB=-L/usr/local/lib 
MAC_BULLSHIT=$(MAC_BULLSHIT_INCLUDE) $(MAC_BULLSHIT_LIB)
OPENGL_FLAGS_LMAO=-framework OpenGL -lglfw3 -lglew
PNGWRITER_LINK=-lpngwriter
PNGWRITER_FREETYPE_DEPENDENCY_LOCATION=-I/usr/local/include/freetype2 
SRC_DIR=src
BUILD_DIR=build

all: main application julia
	$(CC) $(BUILD_DIR)/application.o $(BUILD_DIR)/julia.o $(BUILD_DIR)/main.o -o juliaglsl $(MAC_BULLSHIT) $(PNGWRITER_FREETYPE_DEPENDENCY_LOCATION) $(OPENGL_FLAGS_LMAO) $(PNGWRITER_LINK)

main: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) $(SRC_DIR)/main.cpp -I$(SRC_DIR) $(OPENGL_FLAGS_LMAO) $(MAC_BULLSHIT) -o $(BUILD_DIR)/main.o

application: $(SRC_DIR)/glengine/application.cpp
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(MAC_BULLSHIT_INCLUDE) $(PNGWRITER_FREETYPE_DEPENDENCY_LOCATION) src/glengine/application.cpp -o $(BUILD_DIR)/application.o

julia: $(SRC_DIR)/julia/julia.cpp
	$(CC) $(CFLAGS) $(SRC_DIR)/julia/julia.cpp  -I$(SRC_DIR) $(MAC_BULLSHIT) $(PNGWRITER_FREETYPE_DEPENDENCY_LOCATION)  $(OPENGL_FLAGS_LMAO) $(PNGWRITER_LINK) -o $(BUILD_DIR)/julia.o

clean:
	rm juliaglsl *.o *.png *.bmp *.jpg
