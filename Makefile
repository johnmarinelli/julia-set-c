GCC = gcc
GPP = g++
CFLAGS = -Wall -lm -lpng -lz
INCLUDE_DIR = lib/include
JULIA_INCLUDE_DIR = $(INCLUDE_DIR)/julia
OUTPUT_BIN_NAME = julia
OUTPUT_TEST_BIN_NAME = test_julia
SRCS_DIR = lib/src
JULIA_SRCS_DIR = $(SRCS_DIR)/julia
OBJFILES_DIR = dev/objfiles

all: main.o julia.o
	$(GCC) $(OBJFILES_DIR)/main.o $(OBJFILES_DIR)/julia.o $(CFLAGS) -o $(OUTPUT_BIN_NAME)

compile: main.o julia.o
	$(GCC) -c $(OBJFILES_DIR)/main.o $(OBJFILES_DIR)/julia.o

test: julia.o
	$(GPP) -I$(JULIA_INCLUDE_DIR) -I$(GTEST_DIR)/include test/testmain.cc test/julia_test.cc $(JULIA_SRCS_DIR)/julia.c $(GTEST_ARCHIVE) $(GTEST_MAIN_ARCHIVE) $(CFLAGS) -lpthread -o $(OUTPUT_TEST_BIN_NAME)

main.o: $(SRCS_DIR)/main.c
	$(GCC) -c -I$(INCLUDE_DIR) $(SRCS_DIR)/main.c -o $(OBJFILES_DIR)/main.o

julia.o: $(JULIA_SRCS_DIR)/julia.c
	$(GCC) -c -I$(JULIA_INCLUDE_DIR) $(JULIA_SRCS_DIR)/julia.c -o $(OBJFILES_DIR)/julia.o

