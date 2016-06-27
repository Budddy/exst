#!/usr/bin/env bash

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
cd ./lib_exst/
make clean
make
cd ./tests
make clean
make
./clasp_test
make clean
cd ../
make clean
