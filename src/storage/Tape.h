#pragma once
#include "../config.h"
#include "../tools/FileIO.h"
#include "../models/Record.h"
#include <iostream>
#include <fstream>
#include <vector>

class Tape
{
public:
	Tape(const std::string& filename) : filename(filename) {}
	Tape(int id) : filename("src/data/Tape" + std::to_string(id) + ".bin") {}
	~Tape() = default;

	std::string getFilename() const;

	void write(const std::vector<Record>& records);
	std::streampos read(std::vector<Record>& records, std::streampos position, size_t bytesToRead);

	void print() const;
private:
	std::string filename;
};

