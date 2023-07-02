#pragma once

#include "Trajectory.h"

#include <map>
#include <cmath>

namespace stats {
	template <class M>
	using TrajectoryView = std::vector<std::pair<M, const Trajectory*>>;

	//Search complexity: O(N*log(outputSize)),
	//where N - number of trajectories
	//log(outputSize) comes from the fact that each topList.insert() is log(outputSize)

	template <typename F>
	auto TopMatches(const Trajectory& refer, const TrajectoryStorage& storage, F metrics, unsigned outputSize) {
		const auto refererValue = metrics(refer);

		using MType = std::decay_t<decltype(refererValue)>;
		std::multimap<MType, const Trajectory*> topList;
		TrajectoryView<MType> view;

		for (const auto& tr : storage) {
			auto diffValue = metrics(tr) - refererValue;
			topList.insert( { std::fabs(diffValue), &tr } );

			//Erase the largest element from the trie
			//Search the element --topList.cend() -- constant time (cend is constant and 1 move up)
			//map::erase complexitiy : Amortized constant
			if (topList.size() > outputSize) {
				topList.erase(--topList.cend());
			}
		}

		view.reserve(outputSize);

		for (auto [k, v] : topList) {
			view.push_back({k, v});
		}

		return view;
	}
}
