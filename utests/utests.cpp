#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "serialize.h"
#include "stats.h"

#include <sstream>

TEST_CASE("trajectory with equal times is invalid") 
{
	std::stringstream os;
	os << path_t{{0,1,3}, {5,7,3}, {15,17,3} };

	Trajectory t;
	os >> t;

	CHECK(!t.valid());
}

TEST_CASE("trajectory with one node is invalid")
{
	std::stringstream os;
	os << path_t{ {0,1,3} };

	Trajectory t;
	os >> t;

	CHECK(!t.valid());
}

TEST_CASE("trajectory with 0 nodes is invalid")
{
	std::stringstream os;
	os << path_t{ {0,1,3} };

	Trajectory t;
	os >> t;

	CHECK(!t.valid());
}

TEST_CASE("trajectory is sorted by time")
{
	std::stringstream os;
	os << path_t{ {0,1,10}, {5,7,0} };

	Trajectory t;
	os >> t;

	CHECK(t.valid());
}

TEST_CASE("trajectory length with 2 nodes")
{
	std::stringstream os;
	os << path_t{ {0,1,1}, {3,5,3} };

	Trajectory t;
	os >> t;

	CHECK(t.length() == 5);
	CHECK(t.speed() == 2.5);
}

TEST_CASE("trajectory metrics with 3 nodes")
{
	std::stringstream os;
	os << path_t{ {0,1,1}, {3,5,3}, {6,9,5} };

	Trajectory t;
	os >> t;

	CHECK(t.length() == 10);
	CHECK(t.speed() == 2.5);
}

TEST_CASE("find 2 closest trajectories")
{
	std::stringstream osr;
	auto ref = path_t{ {0,1,1}, {3,6,3} };//5.8 and 2.9
	osr << ref;

	Trajectory tr;
	osr >> tr;

	std::stringstream os;
	os << 3 << " ";
	os << path_t{ {0,1,1}, {3,5,3}, {6,9,5}, {9,13,7} };//15 and 2.5
	os << path_t{ {0,1,1}, {3,5,3}, {6,9,5} };//10 and 2.5
	os << path_t{ {0,1,1}, {3,5,21} };//5 and 0.25

	TrajectoryStorage ts;
	os >> ts;

	auto view = stats::TopMatches(tr, ts, [](auto& t){ return t.length(); }, 2);
	
	REQUIRE(view.size() == 2);
	
	CHECK(std::round(view[0].first) == 1);
	CHECK( view[0].second->path().size() == 2);

	CHECK(std::round(view[1].first) == 4);
	CHECK(view[1].second->path().size() == 3);

	view = stats::TopMatches(tr, ts, [](auto& t) { return t.speed(); }, 2);
	REQUIRE(view.size() == 2);

	CHECK(std::round(view[0].first*10) == 4); //~0.4
	CHECK(view[0].second->path().size() == 4);

	CHECK(std::round(view[1].first*10) == 4);
	CHECK(view[1].second->path().size() == 3);
}
