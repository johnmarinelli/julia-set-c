1) install googletest (i use 1.8.0) https://github.com/google/googletest
2) set $GTEST_DIR to the `googletest` directory of the `googletest` repository (NICE NOMENCLATURE GOOGLE NERDS)
3) `cd $GTEST_DIR && mkdir build && cd build && cmake $GTEST_DIR && make`
4) set $GTEST_ARCHIVE and $GTEST_MAIN_ARCHIVE in $GTEST_DIR/build/{libgtest.a, libgtest_main.a}
5) now `cd $JULIA_SET_C` and `make test` to test
