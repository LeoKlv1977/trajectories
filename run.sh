#!/bin/bash

exe() { echo "[.]\$ $@" ; "$@" ; }

exe bin/tracker -l data data/test_trajectory.dat
echo "--------------------------------------------"
exe bin/tracker -s data data/test_trajectory.dat
echo "--------------------------------------------"

echo "BAD INPUT DATA EXAMPLES:"
echo
exe bin/tracker -x data data/test_trajectory.dat
echo
exe bin/tracker -l dataUnkonw data/test_trajectory.dat
echo
exe bin/tracker -l data dataUnk/test_trajectory.dat
echo
