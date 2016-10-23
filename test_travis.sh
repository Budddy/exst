#!/usr/bin/env bash

./build_test/bin/exst_test
rc=$?
if [[ $rc = 1 ]]; then exit 0;else exit 1;fi