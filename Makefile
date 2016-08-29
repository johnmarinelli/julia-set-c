GCC = gcc
GPP = g++
CFLAGS = -Wall

all: julia.o

test-julia: julia.o
	$(GPP) -I$(GTEST_DIR)/include -Ilib/include test/testmain.cc test/mytest.cc lib/src/my_fn.c $(GTEST_ARCHIVE) $(GTEST_MAIN_ARCHIVE) -o test_julia

julia.o:
	$(GCC) -c -Ilib/include lib/src/julia.c -o dev/objfiles/julia.o

