#!/usr/bin/env bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/travis/build/Budddy/test_exst/build_htd/lib/
./build_test/bin/exst_test
rc=$?
if [[ $rc = 1 ]]; then exit 0;else exit 1;fi