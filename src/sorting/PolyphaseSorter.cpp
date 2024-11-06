#include "PolyphaseSorter.h"

PolyphaseSorter::PolyphaseSorter(Tape* tape1, Tape* tape2, Tape* outputTape)
{
	this->tape1 = tape1;
	this->tape2 = tape2;
	this->outputTape = outputTape;
}

int PolyphaseSorter::getNumberOfSeriesToMerge() {
	return std::min(tape1->getSeriesCounter(), tape2->getSeriesCounter());
}

void PolyphaseSorter::compareRecords(bool& read1Tape, bool& read2Tape)
{
	if (firstTapePrevRecord < secondTapePrevRecord) {
		outputTape->writeRecord(firstTapePrevRecord, true);
		read1Tape = true;
		//firstTapePrevRecord.print();
	}
	else {
		outputTape->writeRecord(secondTapePrevRecord, true);
		read2Tape = true;
		//secondTapePrevRecord.print();
	}
}

void PolyphaseSorter::handleSerieMergeEnd(int& numberOfSeriesToMerge) {
	tape1->decrementSeriesCounter();
	tape1->decrementDummySeriesCounter();

	tape2->decrementSeriesCounter();
	tape2->decrementDummySeriesCounter();

	outputTape->incrementSeriesCounter();

	isFirstTapeSerieEnd = false;
	isSecondTapeSerieEnd = false;

	numberOfSeriesToMerge--;
}

void PolyphaseSorter::readTape(Tape* tape, Record& prevRecord, bool& isSerieEnd, bool& readTape, bool& fileEof)
{
	if (tape->getDummySeriesCounter() > 0) {
		isSerieEnd = true;
	}
	else if (readTape && fileEof) {
		Record tmpRecord;
		fileEof = tape->readRecord(tmpRecord, true);
		isSerieEnd = tmpRecord < prevRecord;
		prevRecord = tmpRecord;
		readTape = false;
	}
	else if (!fileEof) {
		isSerieEnd = true;
	}
}

void PolyphaseSorter::processBlocks(int& numberOfSeriesToMerge)
{
	while (numberOfSeriesToMerge > 0 && (fileEof1 || fileEof2)) {
		readTape(tape1, firstTapePrevRecord, isFirstTapeSerieEnd, read1Tape, fileEof1);
		readTape(tape2, secondTapePrevRecord, isSecondTapeSerieEnd, read2Tape, fileEof2);

		if (!isFirstTapeSerieEnd && !isSecondTapeSerieEnd) {
			compareRecords(read1Tape, read2Tape);
		}
		else if (isFirstTapeSerieEnd && !isSecondTapeSerieEnd) {
			outputTape->writeRecord(secondTapePrevRecord, true);
			read2Tape = true;
			//secondTapePrevRecord.print();
		}
		else if (!isFirstTapeSerieEnd && isSecondTapeSerieEnd) {
			outputTape->writeRecord(firstTapePrevRecord, true);
			read1Tape = true;
			//firstTapePrevRecord.print();
		}
		else {
			handleSerieMergeEnd(numberOfSeriesToMerge);
			//std::cout << "END OF SERIE\n";
		}
	}

}

void PolyphaseSorter::swapTapes()
{
	Tape* temp = outputTape;
	outputTape = (tape1->getSeriesCounter() == 0) ? tape1 : tape2;
	if (tape1->getSeriesCounter() == 0) {
		tape1 = temp;
	}
	else {
		tape2 = temp;
	}
}

void PolyphaseSorter::resetTapeStates() {
	//firstTapePrevRecord = Record();
	//secondTapePrevRecord = Record();

	isFirstTapeSerieEnd = false;
	isSecondTapeSerieEnd = false;

	if (firstTapePrevRecord.getParallelogram().area() == 0) read1Tape = true;
	if (secondTapePrevRecord.getParallelogram().area() == 0) read2Tape = true;

	fileEof1 = true;
	fileEof2 = true;

	outputTape->close();
	outputTape->resetPosition();
	outputTape->open({ std::ios::binary, std::ios::out, std::ios::in, std::ios::trunc });
}

void PolyphaseSorter::printTapeState() {
	if (PrintManager::promptUserDisplayTapesDecision()) {
		PrintManager::printTapeHeader("TAPE 1");
		tape1->print();
		PrintManager::printTapeHeader("TAPE 1");
		PrintManager::printTapeHeader("TAPE 2");
		tape2->print();
		PrintManager::printTapeHeader("TAPE 2");
		PrintManager::printTapeHeader("TAPE 3 (OUTPUT)");
		outputTape->print();
		PrintManager::printTapeHeader("TAPE 3 (OUTPUT)");
	}
}

bool PolyphaseSorter::areTapesEmpty() {
	return tape1->getSeriesCounter() == 0 && tape2->getSeriesCounter() == 0;
}

void PolyphaseSorter::handlePhaseEnd(int& phaseNumber, bool& shouldPrintPhaseHeader, const bool& shouldPrintPhases) 
{
	if (shouldPrintPhases) {
		PrintManager::printPhaseHeader(phaseNumber, tape1->getSeriesCounter(), tape1->getDummySeriesCounter(),
			tape2->getSeriesCounter(), tape2->getDummySeriesCounter(), outputTape->getSeriesCounter(), outputTape->getDummySeriesCounter());
	}
	shouldPrintPhaseHeader = true;
	phaseNumber++;
	if (outputTape->getRecordsToWriteSize() > 0) {
		outputTape->flush(true);
	}
	swapTapes();
	resetTapeStates();
}

void PolyphaseSorter::handlePhaseHeaderPrint(const int& phaseNumber, bool& shouldPrintPhaseHeader, const bool& shouldPrintPhases) 
{
	if (shouldPrintPhases) {
		PrintManager::printPhaseHeader(phaseNumber, tape1->getSeriesCounter(), tape1->getDummySeriesCounter(),
			tape2->getSeriesCounter(), tape2->getDummySeriesCounter(), outputTape->getSeriesCounter(), outputTape->getDummySeriesCounter());
		printTapeState();
		shouldPrintPhaseHeader = true;
	}
	shouldPrintPhaseHeader = false;
}

int PolyphaseSorter::sortTapesWithFibonacci()
{
	PrintManager::printStageHeader("STARTING SORTING");

	int phaseNumber = 1;
	bool shouldPrintPhases = PrintManager::promptUserDisplayPhasesDecision();
	bool shouldPrintPhaseHeader = shouldPrintPhases;

	while (true) {
		int numberOfSeriesToMerge = getNumberOfSeriesToMerge();

		if (shouldPrintPhaseHeader) handlePhaseHeaderPrint(phaseNumber, shouldPrintPhaseHeader, shouldPrintPhases);

		processBlocks(numberOfSeriesToMerge);

		if (areTapesEmpty()) break;
		
		if (numberOfSeriesToMerge == 0) handlePhaseEnd(phaseNumber, shouldPrintPhaseHeader, shouldPrintPhases);
	}
	if (outputTape->getRecordsToWriteSize() > 0) {
		outputTape->flush(true);
	}
	if (shouldPrintPhases) {
		PrintManager::printPhaseHeader(phaseNumber, tape1->getSeriesCounter(), tape1->getDummySeriesCounter(),
			tape2->getSeriesCounter(), tape2->getDummySeriesCounter(), outputTape->getSeriesCounter(), outputTape->getDummySeriesCounter());
	}

	PrintManager::printStageHeader("FINISHING SORTING");

	PrintManager::printTapeHeader("SORTED FILE");
	//outputTape->print();
	PrintManager::printTapeHeader("SORTED FILE");

	return phaseNumber;
}
