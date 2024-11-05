#pragma once
#include "DistributionManager.h"
#include "PolyphaseSorter.h"
#include "../tools/PrintManager.h"
#include <filesystem>

class SortingManager
{
private:
	static void distribute(Tape* read_tape, Tape* tape1, Tape* tape2);
	static int sort(Tape* tape1, Tape* tape2, Tape* outputTape);
public:
	static void sortDataFromFile(const std::string& read_filename, const std::string& tape1_filename, 
		const std::string& tape2_filename, const std::string& tape3_filename);
};

