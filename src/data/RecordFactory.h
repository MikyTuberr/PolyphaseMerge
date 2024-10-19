#pragma once
#include "./../models/Record.h"
#include "./../data/FileIO.h"
#include <random>
#include <iostream>

class RecordFactory
{
public:
	static void createRandomRecords(const std::string& filename, int count);
	static void createManualRecords(const std::string& filename, int count);
};

