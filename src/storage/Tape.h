#pragma once
#include "../models/Record.h"
#include <iostream>
#include <fstream>
#include <initializer_list>
#include <vector>

class Tape
{
public:
	Tape(const std::string& filename) : filename(filename) {};
	~Tape();

	bool isOpen() const;
	bool open(const std::initializer_list<std::ios::openmode> modes);
	void close();

	bool write(const std::vector<Record>& records);
	bool read(std::vector<Record>& records, std::size_t numberOfRecords);

private:
	std::string filename;
	std::fstream file;
};

