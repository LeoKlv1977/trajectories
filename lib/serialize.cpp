#include "serialize.h"

#include <iostream>
#include <fstream>

#include <filesystem>

std::istream& operator >> (std::istream& in, TimePoint& tp)
{
	in >> tp.x;
	in >> tp.y;
	in >> tp.t;
	return in;
}

std::ostream& operator << (std::ostream& out, const TimePoint& tp) {
	out << tp.x << " ";
	out << tp.y << " ";
	out << tp.t;
	return out;
}

std::ostream& operator << (std::ostream& out, const path_t& p) {
	out << p.size() << " ";

	for (const auto& e : p) {
		out << e << " ";
	}

	return out;
}

std::ostream& operator << (std::ostream& out, const Trajectory& t) {
	return out << t.path();
}

std::istream& operator >> (std::istream& in, Trajectory& t) {
	unsigned trajLength{ 0 };
	in >> trajLength;

	TrajectoryBuilder tb(trajLength);

	TimePoint tp;
	for (unsigned j = 0; j < trajLength; ++j) {
		in >> tp;
		tb.AddPoint(tp);
	}

	t = tb.Make();

	return in;
}

std::istream& operator >> (std::istream& in, TrajectoryStorage& ts) {
	//Read the number of trajectories
	unsigned numOfTraj{ 0 };
	in >> numOfTraj;
	ts.reserve(numOfTraj);

	for (unsigned i = 0; i < numOfTraj; ++i) {
		Trajectory t;
		in >> t;

		if (t.valid()) {
			ts.push_back(std::move(t));
		}
	}

	return in;
}

TrajectoryStorage load(const std::string& filename) {
	TrajectoryStorage storage;
	
	std::ifstream istrm(filename, std::ios::binary);
	istrm.exceptions(std::ios::failbit | std::ios::badbit);

	if (!istrm.is_open()) {
		std::cout << "failed to open " << filename << '\n';
	}
	else {
		try {
			istrm >> storage;
		}
		catch (const std::ios_base::failure& f) {
			std::cerr << "error: data file "<< std::quoted(filename) << " is corrupted, " << f.what() << std::endl;
			storage.clear();//??
		}
	}

	return storage;
}

TrajectoryStorage loadDB(const std::string& path) {
	namespace fs = std::filesystem;

	try {

		const fs::path p{path};

		TrajectoryStorage db;
		if (!fs::exists(p)) {
			return db;
		}

		if (fs::is_regular_file(p)) {
			return load(p.string());
		}

		for (const auto& file : fs::directory_iterator(path)) {
			if (file.path().extension() != ".dat") {
				continue;
			}

			auto storage = load(file.path().string());
			if (!storage.empty()) {
				std::move(storage.begin(), storage.end(), std::back_inserter(db));
			}
			else {
				std::cerr << "error: skip file " << std::quoted(file.path().string()) << "\n";
			}
		}

		return db;
	}
	catch (const fs::filesystem_error& err)
	{
		std::cerr << err.what() << std::endl;
	}

	return {};
}
