// =====================================================================================================
// Search complexity : O(N* log(outputSize))
// where N - number of trajectories
// log(outputSize) comes from the fact that each topList.insert() is log(outputSize)
//
// Function topMatches can be used with any types of base elements and metrics
// =====================================================================================================

#pragma once

#include <map>
#include <cmath>

#include "trajectory.h"

namespace stats {
	template <typename M, typename T>
	using TopView = std::vector<std::pair<M, const T*>>;

	template <typename T, typename F>
	auto topMatches(const T& referer, const std::vector<T>& storage, F metrics, unsigned outputSize) {
		const auto refererValue = metrics(referer);

		using MType = std::decay_t<decltype(refererValue)>;
		std::multimap<MType, const T*> topList;
		TopView<MType, T> view;

		for (const auto& tr : storage) {
			auto diffValue = metrics(tr) - refererValue;
			topList.insert( { std::fabs(diffValue), &tr } );

			//Erase the largest element from the trie
			//Search the element --topList.cend() -- constant time (cend is constant and 1 move up)
			//map::erase complexitiy : Amortized constant
			if (topList.size() > outputSize) {
				topList.erase(std::prev(topList.cend()));
			}
		}

		view.reserve(outputSize);

		for (auto [k, v] : topList) {
			view.push_back({k, v});
		}

		return view;
	}
}
