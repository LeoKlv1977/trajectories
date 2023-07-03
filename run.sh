bin/tracker -l data data/test_trajectory.dat
echo "--------------------------------------------"
bin/tracker -s data data/test_trajectory.dat
echo "--------------------------------------------"

echo "BAD INPUT DATA EXAMPLES:"
echo
bin/tracker -x data data/test_trajectory.dat
echo
bin/tracker -l dataUnkonw data/test_trajectory.dat
echo
bin/tracker -l data dataUnk/test_trajectory.dat
echo
