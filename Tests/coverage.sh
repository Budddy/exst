#!/usr/bin/env bash

cd ..
mkdir build_coverage
cd build_coverage
cmake -Dtests=ON ..
make
./bin/exst_test

cd ..
mkdir coverage
cd coverage
gcov ../build_coverage/lib_exst/CMakeFiles/lib_exst.dir/src/*.cpp.o
lcov --capture --directory ../build_coverage/lib_exst/CMakeFiles/lib_exst.dir/src/ --output-file coverage.info
genhtml coverage.info --output-directory out