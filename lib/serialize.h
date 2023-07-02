#pragma once

#include <string>

#include "Trajectory.h"

std::istream& operator >> (std::istream& in, TimePoint& tp);
std::ostream& operator << (std::ostream& out, const TimePoint& tp);

std::istream& operator >> (std::istream& in, Trajectory& t);
std::ostream& operator << (std::ostream& out, const Trajectory& t);

std::istream& operator >> (std::istream& in, TrajectoryStorage& t);

std::ostream& operator << (std::ostream& out, const path_t& p);

TrajectoryStorage load(const std::string& filename);
TrajectoryStorage loadDB(const std::string& path);
