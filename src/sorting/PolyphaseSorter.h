#pragma once
#include "../models/Record.h"
#include "Tape.h"

class PolyphaseSorter
{
private:
	void mergeSeries(const std::vector<Record>& tape1Records, const std::vector<Record>& tape2Records);
	void writeUntilSeriesEnd(const std::vector<Record>& records, size_t& index, bool& seriesEnd, Record& previousRecord);
	void writeRemainingRecords(const std::vector<Record>& records, size_t& index);
	void updateCounters();
public:
	PolyphaseSorter(Tape* tape1, Tape* tape2, Tape * outputTape);
	void sortTapesWithFibonacci();
private:
	Tape *tape1, *tape2, *outputTape;
	Record firstTapePreviousRecord, secondTapePreviousRecord;
	bool isFirstTapeSerieEnd = false, isSecondTapeSerieEnd = false;
};

