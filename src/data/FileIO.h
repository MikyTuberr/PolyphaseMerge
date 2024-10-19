#pragma once
#include "../models/Record.h"
#include <fstream>

class FileIO
{
public:
	static std::streampos read(const std::string& filename, std::vector<Record>& records, std::streampos position, size_t bytesToRead);
	static bool write(const std::string& filename, const std::vector<Record>& records);
};

