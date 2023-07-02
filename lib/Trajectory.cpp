#include "Trajectory.h"

#include <stdexcept>
#include <algorithm>
#include <cmath>


Trajectory::Trajectory(path_t&& p)
	: path_{ std::move(p) }
{
	totalTime_ = getTotalTime();
}

bool Trajectory::valid() const noexcept {
	return path_.size() >= MINIMUN_SIZE && totalTime_ > 0;
}

double Trajectory::getLength() const noexcept {
	assert(path_.size() >= MINIMUN_SIZE);

	double totalLength{ 0 };

	auto lastIter = path_.cbegin();
	auto iter = std::next(lastIter);

	for (; iter != path_.cend(); ++iter) {
		totalLength += std::hypot(iter->x - lastIter->x, iter->y - lastIter->y);
		lastIter = iter;
	}

	return totalLength;
}

int Trajectory::getTotalTime() const noexcept {
	return path_.crbegin()->t - path_.cbegin()->t;
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

	return Trajectory(std::move(path_));
}
