// ===============================================================
// Serialization/load functions
// ===============================================================

#pragma once

#include <string>

#include "trajectory.h"

std::istream& operator >> (std::istream& in, TimePoint& tp);
std::ostream& operator << (std::ostream& out, const TimePoint& tp);

std::istream& operator >> (std::istream& in, Trajectory& t);
std::ostream& operator << (std::ostream& out, const Trajectory& t);

std::istream& operator >> (std::istream& in, TrajectoryStorage& t);

std::ostream& operator << (std::ostream& out, const TPointsPath& p);

//Load trajectories from file set in <filename>
TrajectoryStorage load(const std::string& filename, std::string* error = nullptr);

//Load trajectories from file or directory set in <path>
TrajectoryStorage loadDB(const std::string& path, std::string* error = nullptr);
