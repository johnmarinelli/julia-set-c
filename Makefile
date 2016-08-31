GCC = gcc
GPP = g++
CFLAGS = -Wall
INCLUDE_DIR = lib/include
OUTPUT_BIN_NAME = julia
OUTPUT_TEST_BIN_NAME = test_julia

all: main.o julia.o
	$(GCC) $(CFLAGS) dev/objfiles/main.o dev/objfiles/julia.o -o $(OUTPUT_BIN_NAME)

test: julia.o
	$(GPP) -I$(INCLUDE_DIR) -I$(GTEST_DIR)/include test/testmain.cc test/julia_test.cc lib/src/julia.c $(GTEST_ARCHIVE) $(GTEST_MAIN_ARCHIVE) -o $(OUTPUT_TEST_BIN_NAME)

main.o: lib/src/main.c
	$(GCC) -c -I$(INCLUDE_DIR) lib/src/main.c -o dev/objfiles/main.o

julia.o: lib/src/julia.c
	$(GCC) -c -I$(INCLUDE_DIR) lib/src/julia.c -o dev/objfiles/julia.o

