#pragma once
#include <utility>
#include <vector>
#include "../tools/FileIO.h"
#include "../models/Record.h"
#include "Tape.h"

class DistributionManager 
{
private:
	int findFibonacciNumberOfSeries(const int& firstSeriesCounter, const int& secondSeriesCounter);
	void writeRecord(const Record& record);
	void handleSeriesCounters(const Record& record);
	void incrementSeriesCounters();
	void manageTapeTurn();
	void processRecord(const Record& record);
	void handleSerieEnd(const Record& record);
	void finalizeDistribution();
	void addDummySeries();
public:
	DistributionManager(Tape* read_tape, Tape* tape1, Tape* tape2);
	~DistributionManager() = default;
	void distributeSeriesWithFibonacci();
private:
	Record previousRecord;
	Tape *read_tape, *tape1, *tape2;

	bool isSerieEnd = false;
	bool isSerieCoalesced = false;

	bool isFirstTapeTurn = false;
	int numberOfSeriesForTape = 1;
	bool isFirstRecord = true;
};