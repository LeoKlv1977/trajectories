// =====================================================================================================
// Search complexity : O(N* log(outputSize))
// where N - number of trajectories
// log(outputSize) comes from the fact that each topList.insert() is log(outputSize)
//
// Function topMatches can be used with any types of base item and metrics
// storage Types needs to have value_type equal to Base item type
// It works for STL containers like vector, list, set, deque, array
// =====================================================================================================

#pragma once

#include <map>

#include "trajectory.h"

namespace stats {
	template <typename M, typename T>
	using TopView = std::vector<std::pair<M, const T*>>;

	template <typename C, typename T = typename C::value_type, typename F>
	auto topMatches(const T& referer, const C& storage, F metrics, unsigned outputSize) {
		const auto refererValue = metrics(referer);

		using MType = std::decay_t<decltype(refererValue)>;
		std::multimap<MType, const T*> topList;
		TopView<MType, T> view;

		for (const auto& tr : storage) {
			auto diffValue = metrics(tr) - refererValue;
			topList.insert( { std::abs(diffValue), &tr } );

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
