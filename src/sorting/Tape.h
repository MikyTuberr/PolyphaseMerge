#pragma once
#include <fstream>
#include "../models/Record.h"
#include <vector>
#include "../tools/FileIO.h"

class Tape
{
public:
	Tape() = default;
	Tape(const std::string& filename);
	~Tape();

	bool isOpen() const;
	bool open(const std::initializer_list<std::ios::openmode> modes);
	void close();
	bool write(const std::vector<Record>& records);
	bool read(std::vector<Record>& records);

	void incrementSeriesCounter();
	void decrementSeriesCounter();
	void incrementRecordsCounter();

	int getSeriesCounter() const;
	int getRecordsCounter() const;

private:
	FileIO io;
	std::string filename;
	std::fstream file;
	int seriesCounter;
	int recordsCounter;
};

