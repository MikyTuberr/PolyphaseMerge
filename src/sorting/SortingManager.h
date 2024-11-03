#pragma once
#include "DistributionManager.h"
#include "PolyphaseSorter.h"
#include <filesystem>

class SortingManager
{
public:
	static void sortDataFromFile(const std::string& read_filename, const std::string& tape1_filename, const std::string& tape2_filename);
};

