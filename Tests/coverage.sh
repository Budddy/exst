#!/usr/bin/env bash

mkdir build_coverage
cd build_coverage
cmake -Dtests=ON ..
make
./bin/exst_test

cd ..
mkdir cov
cd cov
gcov ../build_coverage/lib_exst/CMakeFiles/lib_exst.dir/src/*.cpp.o
lcov --capture --directory ../build_coverage/lib_exst/CMakeFiles/lib_exst.dir/src/ --output-file coverage.info

cd ..
mkdir coverage
cd coverage
genhtml ../cov/coverage.info --branch-coverage --function-coverage --output-directory ./

cd ..
rm ./cov -r
rm ./build_coverage -r