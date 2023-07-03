#!/bin/bash

exe() { echo; echo "[.]\$ $@" ; "$@" ; }

exe bin/tracker -l data data/test_trajectory.dat
echo "--------------------------------------------"
exe bin/tracker -s data data/test_trajectory.dat
echo "--------------------------------------------"

echo "BAD INPUT DATA EXAMPLES:"

exe bin/tracker -x data data/test_trajectory.dat

exe bin/tracker -l dataUnkonw data/test_trajectory.dat

exe bin/tracker -l data dataUnk/test_trajectory.dat

exe bin/tracker -s data data/corrupted/test_trajectory1.dat

exe bin/tracker -s data data/corrupted/test_trajectory123.dat

echo
