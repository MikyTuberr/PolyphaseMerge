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
	void handleSeriesCounters(Record record);
	void incrementSeriesCounters();
	void manageTapeTurn();
public:
	DistributionManager(Tape* read_tape, Tape* tape1, Tape* tape2);
	~DistributionManager() = default;
	void distributeSeriesWithFibonacci();
private:
	Record previousRecord, tape1Tail, tape2Tail;
	Tape *read_tape, *tape1, *tape2;
	bool isSerieFinished = false;
	bool isFirstTapeTurn = true;
	bool isPreviousRecordWritten = false;

	bool isTapeCoalesced = false;
	bool isTape1Written = false;
	bool isTape2Written = false;
};