#pragma once
#include "../config.h"
#include "../models/Record.h"
#include <fstream>

class FileIO
{
public:
	static std::streampos read(std::ifstream& file, std::vector<Record>& records, std::streampos position, size_t bytesToRead);
	static void write(std::ostream& file, const std::vector<Record>& records);
};

