#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h" //utests inline header

#include <sstream>

#include <array>
#include <deque>
#include <list>
#include <set>

#include "serialize.h"
#include "stats.h"

static Trajectory Load(const TPointsPath& p) {
	std::stringstream os;
	os << p;

	Trajectory t;
	os >> t;

	return t;
}

TEST_CASE("empty trajectory is not valid") {
	Trajectory t;
	CHECK(!t.valid());
}

TEST_CASE("trajectory with equal times is invalid") {
	Trajectory t = Load({{0,1,3}, {5,7,3}, {15,17,3}});
	CHECK(!t.valid());
}

TEST_CASE("trajectory with one node is invalid") {
	Trajectory t = Load({ {0,1,3} });
	CHECK(!t.valid());
}

TEST_CASE("trajectory with 0 nodes is invalid") {
	Trajectory t;
	CHECK(!t.valid());
}

TEST_CASE("trajectory is with 2 nodes is valid") {
	Trajectory t = Load({ {0,1,10}, {5,7,0} });
	CHECK(t.valid());
}

TEST_CASE("trajectory length and speed with 2 nodes") {
	Trajectory t = Load({ {0,1,11}, {3,5,1} });

	CHECK(t.length() == 5);
	CHECK(t.speed() == 0.5);
}

TEST_CASE("trajectory metrics with 3 nodes") {
	Trajectory t = Load({ {0,1,1}, {3,5,3}, {6,9,5} } );

	CHECK(t.length() == 10);
	CHECK(t.speed() == 2.5);
}

TEST_CASE("find 2 closest trajectories") {
	Trajectory tr = Load({ {0,1,1}, {3,6,3} });//5.8 and 2.9

	TrajectoryStorage ts;
	ts.push_back( Load({ {0,1,1}, {3,5,3}, {6,9,5}, {9,13,7} } ) ); //15 and 2.5
	ts.push_back( Load({ {0,1,1}, {3,5,3}, {6,9,5} } ) );//10 and 2.5
	ts.push_back( Load({ {0,1,1}, {3,5,21} }));//5 and 0.25

	SUBCASE("use length as metrics") {
		auto view = stats::topMatches(tr, ts, [](auto& t){ return t.length(); }, 2);
	
		REQUIRE(view.size() == 2);
	
		CHECK(std::round(view[0].first) == 1);
		CHECK( view[0].second->path().size() == 2);

		CHECK(std::round(view[1].first) == 4);
		CHECK(view[1].second->path().size() == 3);
	}

	SUBCASE("use speed as metrics, case with 2 equal metrics distances") {
		auto view = stats::topMatches(tr, ts, [](auto& t) { return t.speed(); }, 2);
		REQUIRE(view.size() == 2);

		CHECK(std::round(view[0].first*10) == 4); //~0.4
		CHECK(view[0].second->path().size() == 4);

		CHECK(std::round(view[1].first*10) == 4);
		CHECK(view[1].second->path().size() == 3);
	}
}

TEST_CASE("test find in array with int metrics") {
	int x = 5;

	std::array<int, 5> list = {1, 7, 8, 10, 45};
	auto view = stats::topMatches(x, list, [](auto& t) { return t; }, 2);

	REQUIRE(view.size() == 2);

	CHECK(*view[0].second == 7);
	CHECK(*view[1].second == 8);
}

TEST_CASE_TEMPLATE("test find in containers", T, std::list<int>, std::vector<int>, std::deque<int>, std::set<int>) {
	int x = 5;

	T list = { 1, 7, 8, 10, 45 };
	auto view = stats::topMatches(x, list, [](auto& t) { return t; }, 2);

	REQUIRE(view.size() == 2);

	CHECK(*view[0].second == 7);
	CHECK(*view[1].second == 8);
}

