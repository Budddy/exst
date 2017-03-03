#!/usr/bin/env bash

##########################
# test script for travis #
##########################

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/
./build_test/bin/exst_test
rc=$?
if [[ $rc = 1 ]]; then exit 0;else exit 1;fi