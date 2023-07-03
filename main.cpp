#include <filesystem>
#include <functional>

#include "trajectory.h"
#include "serialize.h"
#include "stats.h"

constexpr unsigned NEAREST_GROUP_SIZE = 3;

int main(int argc, const char** argv)
{	
	if (argc == 0) {
		//Unexpected and hardly possible case
		return 1;
	}
	
	const auto binaryName = std::filesystem::path{ argv[0] }.filename().string();
	if (argc == 1) {
		std::cerr << binaryName << " finds 3 closest trajectories.\n"
				  << "Usage: " << binaryName << " <-l|-s> <db_file|db_directory> <ref_path>" << std::endl;
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

	if (argc != 4) {
		std::cerr << "wrong number of arguments\n" 
			      << "usage: " << binaryName << " " << cmd << " <db_file|db_directory> <ref_path>"
			      << std::endl;

		return 1;
	}

	const std::string refFileName = argv[3];
	auto loadRefResult = load(refFileName);
	if (loadRefResult.first.empty()) {
		std::cerr << "error: reference file "
				  << std::quoted(refFileName) << " " << loadRefResult.second << std::endl;
		return 1;
	}

	const std::string dbFileName = argv[2];
	auto loadDbResult = loadDB(dbFileName);
	const auto& db = loadDbResult.first;

	if (db.empty()) {
		std::cerr << "error: db directory " 
				  << std::quoted(dbFileName) << " "
			      << (loadDbResult.second.empty() ? "is empty" : loadDbResult.second) << std::endl;
		return 1;
	}

	//Metrics function's signature:
	std::function<double (const Trajectory& t)> f;
	const auto& ref = loadRefResult.first.at(0);

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
		std::cout << "\n";
	}

	std::cout << std::endl;

	return 0;
}
