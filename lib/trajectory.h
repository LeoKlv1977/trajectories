// ==================================================================================
// Class Trajectory is responsible for 
// storing data and handling operations with trajectories
// 
// The only way to build a valid Trajectory is to use TrajectoryBuilder
// TrajectoryBuilder's purpose is to create valid Trajectories and handle exceptions
// ==================================================================================

#pragma once

#include <vector>
#include <iostream>
#include <cassert>

struct TimePoint
{
	int x{0};
	int y{0};
	int t{0};
};

using TPointsPath = std::vector<TimePoint>;

/*Trajectory class is movable only to exclude copies of std::vector*/
class Trajectory {
	friend class TrajectoryBuilder;
	friend std::ostream& operator << (std::ostream& out, const Trajectory& t);

	static constexpr double INITIAL = -1;
public:
	static constexpr int MINIMUN_SIZE = 2;

	explicit Trajectory() noexcept = default;

	Trajectory(Trajectory&&) = default;
	Trajectory& operator=(Trajectory&&) = default;

	Trajectory(const Trajectory& ) = delete;
	Trajectory& operator=(const Trajectory&) = delete;

	bool valid() const noexcept;

	double speed() const noexcept {
		if (speed_ == INITIAL) {
			assert(valid());
			speed_ = length() / totalTime_;
		}

		return speed_;
	}

	double length() const noexcept {
		if (length_ == INITIAL) {
			assert(valid());
			length_ = getLength();
		}

		return length_;
	}

	const TPointsPath& path() const noexcept {
		return path_;
	}

private:
	explicit Trajectory(TPointsPath&& p);

	double getLength() const noexcept;
	int getTotalTime() const noexcept;

	mutable double speed_{ INITIAL };
	mutable double length_{ INITIAL };

	int totalTime_{0};

	//Sorted array by Time (t)
	TPointsPath path_;
};

/*TrajectoryBuilder is the only way to create a valid trajectory*/
class TrajectoryBuilder {
public:
	explicit TrajectoryBuilder(unsigned length)
	{
		path_.reserve(length);
	}

	template <typename T>
	TrajectoryBuilder& AddPoint(T&& t) {
		path_.push_back(std::forward<T>(t));
		return *this;
	}

	Trajectory Make();

private:
	TPointsPath path_;
};

using TrajectoryStorage = std::vector<Trajectory>;
