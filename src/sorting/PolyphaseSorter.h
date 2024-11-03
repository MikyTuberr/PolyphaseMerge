#pragma once
#include "../models/Record.h"
#include "Tape.h"

class PolyphaseSorter
{
private:
	int getNumberOfSeriesToMerge();
	bool readBlocksFromTapes();
	bool isEndOfSerie(const std::vector<Record>& records, size_t index);
	void writeToOutputTape(Tape* tape, const std::vector<Record>& records, Record& record, size_t& index, bool& isSerieEndFlag);
	void compareRecords(size_t& i, size_t& j);
	void handleSerieMergeEnd(int& numberOfSeriesToMerge);
	void handleTapeEnd(Tape* tape, std::vector<Record>& records, size_t& index, Record& currentRecord, bool& serieEndFlag, int& numberOfSeriesToMerge);
	void processBlocks(int& numberOfSeriesToMerge);
	void swapTapes();
	void handleRemainingRecords();
public:
	PolyphaseSorter(Tape* tape1, Tape* tape2, Tape * outputTape);
	void sortTapesWithFibonacci();
private:
	Tape *tape1, *tape2, *outputTape;
	size_t tape1Index = 0, tape2Index = 0;
	std::vector<Record> tape1Records, tape2Records;
	Record tape1CurrentRecord, tape2CurrentRecord;
	bool isTape1SerieEnd = false, isTape2SerieEnd = false;
	bool readFromTape1 = true, readFromTape2 = true;
};

