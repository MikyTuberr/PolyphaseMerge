#pragma once
#include "../models/Record.h"
#include "Tape.h"
#include "../tools/PrintManager.h"

class PolyphaseSorter
{
private:
	int getNumberOfSeriesToMerge();
	void compareRecords(bool& read1Tape, bool& read2Tape);
	void handleSerieMergeEnd(int& numberOfSeriesToMerge);
	void readTape(Tape* tape, Record& prevRecord, bool& isSerieEnd, bool& readTape, bool& fileEof);
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
	Record  firstTapePrevRecord, secondTapePrevRecord;
	bool isFirstTapeSerieEnd = false, isSecondTapeSerieEnd = false;
	bool fileEof1 = true, fileEof2 = true;
	bool read1Tape = true;
	bool read2Tape = true;
};

