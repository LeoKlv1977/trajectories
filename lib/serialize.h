#pragma once

#include <string>

#include "Trajectory.h"

std::istream& operator >> (std::istream& in, TimePoint& tp);
std::ostream& operator << (std::ostream& out, const TimePoint& tp);

std::istream& operator >> (std::istream& in, Trajectory& t);
std::ostream& operator << (std::ostream& out, const Trajectory& t);

std::istream& operator >> (std::istream& in, TrajectoryStorage& t);

std::ostream& operator << (std::ostream& out, const path_t& p);

TrajectoryStorage Load(const std::string& filename);
TrajectoryStorage LoadDb(const std::string& path);
