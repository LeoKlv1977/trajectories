#include <iostream>
#include <fstream>
#include <filesystem>

#include "serialize.h"

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

std::ostream& operator << (std::ostream& out, const TPointsPath& p) {
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
	unsigned ntraj{ 0 };
	in >> ntraj;
	ts.reserve(ntraj);

	for (unsigned i = 0; i < ntraj; ++i) {
		Trajectory t;
		in >> t;

		if (t.valid()) {
			ts.push_back(std::move(t));
		}
	}

	return in;
}

std::pair<TrajectoryStorage, std::string> load(const std::string& filename) {
	TrajectoryStorage storage;
	
	std::ifstream istrm(filename, std::ios::binary);
	if (!istrm.is_open()) {
		return { std::move(storage), "is not opened" };
	}

	try {
		istrm.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
		istrm >> storage;
	}
	catch (const std::ios_base::failure&) {
		storage.clear();
		return { std::move(storage), "is corrupted" };
	}

	return { std::move(storage), ""};
}

std::pair<TrajectoryStorage, std::string> loadDB(const std::string& path) {
	namespace fs = std::filesystem;
	
	TrajectoryStorage db;

	try {

		const fs::path p{path};

		if (!fs::exists(p)) {
			return { std::move(db), "does not exist"};
		}
		else if (fs::is_regular_file(p)) {
			return load(p.string());
		}
		else {
			for (const auto& file : fs::directory_iterator(path)) {
				if (file.path().extension() != ".dat") {
					continue;
				}

				auto r = load(file.path().string());
			
				auto& storage = r.first;
				if (!storage.empty()) {
					std::move(storage.begin(), storage.end(), std::back_inserter(db));
				}
			}
		}
	}
	catch (const fs::filesystem_error& err)
	{
		std::cerr << "unexpected error: " << err.what() << std::endl;
		db.clear();
	}

	return { std::move(db), "" };
}
