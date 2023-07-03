#include <algorithm>
#include <cmath>

#include "trajectory.h"


Trajectory::Trajectory(TPointsPath&& p)
	: path_{ std::move(p) }
{
	totalTime_ = getTotalTime(path_);
}

bool Trajectory::valid() const noexcept {
	return path_.size() >= MINIMUN_SIZE;
}

double Trajectory::getLength() const noexcept {
	assert(path_.size() >= MINIMUN_SIZE);

	double totalLength{ 0 };

	for (auto lastIter = path_.cbegin(), iter = std::next(lastIter); iter != path_.cend(); ++iter) {
		totalLength += std::hypot(iter->x - lastIter->x, iter->y - lastIter->y);
		lastIter = iter;
	}

	return totalLength;
}

int Trajectory::getTotalTime(const TPointsPath& p) noexcept {
	return p.crbegin()->t - p.cbegin()->t;
}

Trajectory TrajectoryBuilder::Make() {
	if (path_.size() < Trajectory::MINIMUN_SIZE) {
		//Num of Points less then required
		return Trajectory{};
	}

	std::sort(path_.begin(), path_.end(),
		[](const auto& tp1, const auto& tp2) {
			return tp1.t < tp2.t;
		}
	);

	auto time = Trajectory::getTotalTime(path_);
	if (time == 0) {
		return Trajectory{};
	}

	return Trajectory(std::move(path_));
}
