GCC = gcc
GPP = g++
CFLAGS = -Wall -lm
INCLUDE_DIR = lib/include
OUTPUT_BIN_NAME = julia
OUTPUT_TEST_BIN_NAME = test_julia
SRCS_DIR = lib/src
OBJFILES_DIR = dev/objfiles

all: main.o julia.o
	$(GCC) $(CFLAGS) $(OBJFILES_DIR)/main.o $(OBJFILES_DIR)/julia.o -o $(OUTPUT_BIN_NAME)

compile: main.o julia.o
	$(GCC) -c $(OBJFILES_DIR)/main.o $(OBJFILES_DIR)/julia.o

test: julia.o
	$(GPP) -I$(INCLUDE_DIR) -I$(GTEST_DIR)/include test/testmain.cc test/julia_test.cc $(SRCS_DIR)/julia.c $(GTEST_ARCHIVE) $(GTEST_MAIN_ARCHIVE) -o $(OUTPUT_TEST_BIN_NAME)

main.o: $(SRCS_DIR)/main.c
	$(GCC) -c -I$(INCLUDE_DIR) $(SRCS_DIR)/main.c -o $(OBJFILES_DIR)/main.o

julia.o: $(SRCS_DIR)/julia.c
	$(GCC) -c -I$(INCLUDE_DIR) $(SRCS_DIR)/julia.c -o $(OBJFILES_DIR)/julia.o

