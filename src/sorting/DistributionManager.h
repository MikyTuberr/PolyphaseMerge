#pragma once
#include <utility>
#include <vector>
#include "../tools/FileIO.h"
#include "../models/Record.h"
#include "Tape.h"

class DistributionManager 
{
private:
	std::pair<int, int> findFibonacciPair(int firstSeriesCounter, int secondSeriesCounter);
	void writeRecord(Record record);
	void incrementSeriesCounter();
	void manageTapeTurn();
public:
	DistributionManager(Tape* read_tape, Tape* tape1, Tape* tape2);
	~DistributionManager() = default;
	void distributeSeriesWithFibonacci();
private:
	Record previousRecord;
	Tape *read_tape, *tape1, *tape2;
	bool isSerieFinished = false;
	bool isFirstTapeTurn = true;
	bool isPreviousRecordWritten = false;
};