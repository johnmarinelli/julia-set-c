GCC = gcc
GPP = g++
CFLAGS = -Wall -lm -lpng -lz -lpthread -g
APPLE_OPENGL_FLAGS = -framework OpenGL -framework GLUT
INCLUDE_DIR = lib/include
OUTPUT_BIN_NAME = mkjl
OUTPUT_TEST_BIN_NAME = test_julia
OUTPUT_BENCH_BIN_NAME = bench_julia
SRCS_DIR = lib/src
OBJFILES_DIR = dev/objfiles

all: main.o julia.o
	$(GCC) -I$(INCLUDE_DIR) $(OBJFILES_DIR)/main.o $(OBJFILES_DIR)/julia.o $(CFLAGS) -O3 -o $(OUTPUT_BIN_NAME)

compile: main.o julia.o
	$(GCC) -c $(OBJFILES_DIR)/main.o $(OBJFILES_DIR)/julia.o

bench: julia.o
	$(GCC) -I$(INCLUDE_DIR) -Ibenchmark/ $(SRCS_DIR)/julia/julia.c benchmark/bench_all.c $(CFLAGS) -O0 -o $(OUTPUT_BENCH_BIN_NAME)

test: julia.o
	$(GPP) -I$(INCLUDE_DIR) -I$(GTEST_DIR)/include test/testmain.cc test/julia_test.cc $(SRCS_DIR)/julia/julia.c $(GTEST_ARCHIVE) $(GTEST_MAIN_ARCHIVE) $(CFLAGS) -lpthread -o $(OUTPUT_TEST_BIN_NAME)

main.o: $(SRCS_DIR)/main.c
	$(GCC) -c -I$(INCLUDE_DIR) $(SRCS_DIR)/main.c $(APPLE_OPENGL_FLAGS) -o $(OBJFILES_DIR)/main.o -g

julia.o: $(SRCS_DIR)/julia/julia.c
	$(GCC) -c -I$(INCLUDE_DIR) $(SRCS_DIR)/julia/julia.c -o $(OBJFILES_DIR)/julia.o -g

