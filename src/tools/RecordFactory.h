#pragma once
#include "../config.h"
#include "./../models/Record.h"
#include "./../tools/FileIO.h"
#include <random>
#include <iostream>

class RecordFactory
{
public:
	static void createRandomRecords(const std::string& filename, int recordsNumber);
	static void createManualRecords(const std::string& filename, int recordsNumber);
};

