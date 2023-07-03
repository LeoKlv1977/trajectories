# find closest trajectories

To (re)build the project from scratch run build.sh

Binaries are located in bin directory

bin/tracker: name of the executable
bin/utests: unittests

run.sh: some examples how to run tracker, also there are examples with bad input

# Description of the project

The project is crossplatform, requires C++ 17 or higher. Build system is Cmake. To (re)build the project from scratch run build.sh
Algorithms are covered by unitests, that are implemented using doctest project. This is not what I use in real life, but I liked the idea to build tests using header file only.

The key class is Trajectory, where path of timepoints sorted by time values is stored. It can also calculate metrics of the path: speed, length.
Metrics are calculated once, then they cached. Note that path is constant, so once created Trajectory can not be modified. 
Creation of new valid not empty Trajectories can be done only by TrajectoryBuilder. 

Function that searches closes matches is located in stats.h
stats::topMatches finds M closest entries among N entries in container. Container is a generic type. So, many STL containers like vector, list, set, deque, array can be used.
Value type of the container should be equal to Reference type. Metrics type is any numeric type.
Output type is 'std::vector<std::pair<M, const T*>>', where M is a type of Metrics, T is a type of reference object

COMPLEXITY of searching 3 closest matches is O(N*log(3)), i.e. linear, where N - a number of trajectories in the container.
