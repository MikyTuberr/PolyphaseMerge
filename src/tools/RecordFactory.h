#pragma once
#include "../config.h"
#include "./../models/Record.h"
#include "./../tools/FileIO.h"
#include <random>
#include <iostream>

class RecordFactory
{
public:
	static void createRandomRecords(FileIO& io, std::ofstream& filename, int recordsNumber);
	static void createManualRecords(FileIO& io, std::ofstream& filename, int recordsNumber);
};

