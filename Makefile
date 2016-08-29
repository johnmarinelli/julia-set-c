test-julia:
	g++ -I$(GTEST_DIR)/include -Ilib/include test/testmain.cc test/mytest.cc lib/src/my_fn.c $(GTEST_ARCHIVE) $(GTEST_MAIN_ARCHIVE) -o test_julia
