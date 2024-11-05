#pragma once
#include "../models/Record.h"
#include "Tape.h"
#include "../tools/PrintManager.h"

class PolyphaseSorter
{
private:
	int getNumberOfSeriesToMerge();
	bool isEndOfSerie(const std::vector<Record>& records, size_t index);
	void writeToOutputTape(Tape* tape, const std::vector<Record>& records, Record& record, 
		size_t& index, bool& isSerieEndFlag);
	void compareRecords(size_t& i, size_t& j);
	void handleSerieMergeEnd(int& numberOfSeriesToMerge);
	void readTape(Tape* tape, std::vector<Record>& records, size_t& index, bool& readTape);
	void checkTapeState(std::vector<Record>& records, Record& currentRecord, size_t& index,
		int dummySeriesCount, bool& isSerieEnd, bool& readTape);
	void processBlocks(int& numberOfSeriesToMerge);
	void swapTapes();
	void resetTapeStates();
	void printTapeState();
	bool areTapesEmpty();
	void handlePhaseEnd(int& phaseNumber, bool& shouldPrintPhaseHeader, const bool& shouldPrintPhases);
	void handlePhaseHeaderPrint(const int& phaseNumber, bool& shouldPrintPhaseHeader, const bool& shouldPrintPhases);
public:
	PolyphaseSorter(Tape* tape1, Tape* tape2, Tape * outputTape);
	int sortTapesWithFibonacci();
private:
	Tape *tape1, *tape2, *outputTape;
	size_t tape1Index = 0, tape2Index = 0;
	std::vector<Record> tape1Records, tape2Records;
	Record tape1CurrentRecord, tape2CurrentRecord;
	bool isTape1SerieEnd = false, isTape2SerieEnd = false;
};

