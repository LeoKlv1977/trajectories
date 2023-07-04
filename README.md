# Find closest trajectories

### Hints on how to build and run

To (re)build the project from scratch run _build.sh_. You many need to run ```chmod a+x *.sh``` before.

Binaries are located in bin directory

bin/tracker: name of the executable

bin/utests: unittests

run.sh: some examples how to run tracker, also there are examples with bad input

### Description

The project is crossplatform, requires C++ 17 or higher. Build system is Cmake. To (re)build the project from scratch run build.sh.
The code is covered by unitests that are implemented using doctest project. This is not what I use in real life, but I liked the idea to build tests using header file only.

The key class is Trajectory, where path of timepoints sorted by time is stored. It can also calculate metrics of the path: speed, length.
Metrics are lazily calculated once, then they are cached. Note that in fact path is constant, so once created path can not be modified, so its metrics are also constant.

Creation of new valid not empty Trajectory object can be done only by TrajectoryBuilder. Trajectory can be either valid or empty.

Function **stats::topMatches**, that searches the closest matches, is located in _stats.h_. It finds _M_ closest entries among _N_ entries in a _container_. _Container_ is a generic iterable type which holds objects of type equal to the type of _Reference_. Many STL containers like vector, list, set, deque, array are supported.
_Metrics type_ is any numeric type.
Output type is _std::vector<std::pair<M, const T*>>_, where _M_ is a type of Metrics, _T_ is a type of _reference object_

**COMPLEXITY** of searching 3 closest matches is O(N*log(3)), i.e. linear, where N - a number of trajectories in the container.

### Run from the commandline
If you need to see a syntax of _tracker_ then just launch it with empty list of arguments:

```
[onelink@nvmcent64v2 trajectories]$ bin/tracker
tracker finds 3 closest trajectories.
Usage: tracker <-l|-s> <db_file|db_directory> <ref_path>
```
if _command_ is wrong then you get this message:
```
[onelink@nvmcent64v2 trajectories]$ bin/tracker -x
unrecognized command: '-x'
```
if _command_ has wrong number of arguments:
```
[onelink@nvmcent64v2 trajectories]$ bin/tracker -l arg
wrong number of arguments
usage: tracker -l <db_file|db_directory> <ref_path>
```

If input format of dat files is wrong or I/O errors occurs then you will see a corresponding message:
```
[.]$ bin/tracker -l dataUnkonw data/test_trajectory.dat
error: db directory "dataUnkonw" does not exist

[.]$ bin/tracker -l data dataUnk/test_trajectory.dat
error: reference file "dataUnk/test_trajectory.dat" is not opened

[.]$ bin/tracker -s data data/corrupted/test_trajectory1.dat
error: reference file "data/corrupted/test_trajectory1.dat" is corrupted

[.]$ bin/tracker -s data data/corrupted/test_trajectory123.dat
error: reference file "data/corrupted/test_trajectory123.dat" is not opened

[.]$ bin/tracker -s build data/test_trajectory.dat
error: db directory "build" is empty
```
