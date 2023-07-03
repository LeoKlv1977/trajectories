#include "Trajectory.h"
#include "serialize.h"
#include "stats.h"

#include <iostream>
#include <filesystem>
#include <functional>

constexpr unsigned NEAREST_GROUP_SIZE = 3;

int main(int argc, const char** argv)
{	
	if (argc != 4) {
		std::cerr << "wrong number of input arguments, expected:\n";
		std::cerr << std::filesystem::path{argv[0]}.filename().string()
				  << " <-l|-s> <db_file|db_directory> <ref_path>" << std::endl;
		return 1;
	}

	const std::string cmd = argv[1];
	std::string cmdDescription;

	if (cmd == "-l") {
		cmdDescription = "LENGTH";
	}
	else if (cmd == "-s") {
		cmdDescription = "SPEED";
	}
	else {
		std::cerr << "unrecognized command: '" << cmd << "'" << std::endl;
		return 1;
	}

	const std::string refFileName = argv[3];
	auto refs = load(refFileName);
	if (refs.empty()) {
		std::cerr << "ref file " << refFileName << " is empty or corrupted";
		return 1;
	}

	const std::string dbFileName = argv[2];
	auto db = loadDB(dbFileName);
	if (db.empty()) {
		std::cerr << "db '" << dbFileName << "' is empty or corrupted";
		return 1;
	}

	//Metrics function's signature:
	std::function<double (const Trajectory& t)> f;
	const auto& ref = refs.at(0);

	if (cmd == "-l") {
		f = [](const Trajectory& t) { return t.length(); };
	}
	else if (cmd == "-s") {
		f = [](const Trajectory& t) { return t.speed(); };
	}

	auto view = stats::topMatches(ref, db, f, NEAREST_GROUP_SIZE);

	auto nicePrint = [](const TPointsPath& p) {
		for (const auto& e : p) {
			std::cout << "( " << e << " ) ";
		}
	};

	std::cout << "\nReference : \n\t";
	nicePrint(ref.path());
	std::cout << "\n\n";

	std::cout << "Closest Trajectories by metrics '" << cmdDescription << "' :\n\n";

	unsigned counter{0};
	for (const auto& e : view) {
		std::cout << ++counter << ". Metrics difference : " << e.first << "\n";

		std::cout << "\t";
		nicePrint(e.second->path());
		std::cout << "\n\n";
	}

	return 0;
}

