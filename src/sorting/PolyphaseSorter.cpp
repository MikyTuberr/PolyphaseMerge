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

bool PolyphaseSorter::isEndOfSerie(const std::vector<Record>& records, size_t index) {
	if (index + 1 < records.size()) {
		return records[index] > records[index + 1];
	}
	return false;
}

void PolyphaseSorter::writeToOutputTape(Tape* tape, const std::vector<Record>& records, Record& record, size_t& index, bool& isSerieEndFlag) {
	outputTape->writeRecord(record);
	//record.print();
	isSerieEndFlag = isEndOfSerie(records, index);
	index++;
}

void PolyphaseSorter::compareRecords(size_t& i, size_t& j)
{
	if (tape1CurrentRecord < tape2CurrentRecord) {
		writeToOutputTape(tape1, tape1Records, tape1CurrentRecord, i, isTape1SerieEnd);
	}
	else {
		writeToOutputTape(tape1, tape2Records, tape2CurrentRecord, j, isTape2SerieEnd);
	}
}

void PolyphaseSorter::handleSerieMergeEnd(int& numberOfSeriesToMerge) {
	tape1->decrementSeriesCounter();
	tape1->decrementDummySeriesCounter();
	tape2->decrementSeriesCounter();
	tape2->decrementDummySeriesCounter();
	outputTape->incrementSeriesCounter();
	numberOfSeriesToMerge--;
	isTape1SerieEnd = false;
	isTape2SerieEnd = false;
}

void PolyphaseSorter::readTape(Tape* tape, std::vector<Record>& records, size_t& index, bool& readTape)
{
	if (readTape && index == records.size()) {
		records = std::vector<Record>();
		index = 0;
		readTape = tape->read(records);
	}
}

void PolyphaseSorter::checkTapeState(std::vector<Record>& records, Record& currentRecord, size_t& index,
	int dummySeriesCount, bool& isSerieEnd, bool& readTape)
{
	if (!readTape) {
		isSerieEnd = true;
		currentRecord = Record();
	}
	else if (dummySeriesCount > 0) {
		isSerieEnd = true;
	}
	else if (index == 0 && currentRecord != records[index]) {
		isSerieEnd = currentRecord > records[index];
		currentRecord = records[index];
	}
	else {
		currentRecord = records[index];
	}
}

void PolyphaseSorter::processBlocks(int& numberOfSeriesToMerge)
{
	bool read1Tape = true;
	bool read2Tape = true;

	while (numberOfSeriesToMerge > 0 && (read1Tape || read2Tape)) {

		readTape(tape1, tape1Records, tape1Index, read1Tape);
		readTape(tape2, tape2Records, tape2Index, read2Tape);

		checkTapeState(tape1Records, tape1CurrentRecord, tape1Index, tape1->getDummySeriesCounter(), isTape1SerieEnd, read1Tape);
		checkTapeState(tape2Records, tape2CurrentRecord, tape2Index, tape2->getDummySeriesCounter(), isTape2SerieEnd, read2Tape);

		if (!isTape1SerieEnd && !isTape2SerieEnd) {
			compareRecords(tape1Index, tape2Index);
		}
		else if (isTape1SerieEnd && !isTape2SerieEnd) {
			writeToOutputTape(tape2, tape2Records, tape2CurrentRecord, tape2Index, isTape2SerieEnd);
		}
		else if (!isTape1SerieEnd && isTape2SerieEnd) {
			writeToOutputTape(tape1, tape1Records, tape1CurrentRecord, tape1Index, isTape1SerieEnd);
		}
		else {
			handleSerieMergeEnd(numberOfSeriesToMerge);
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
	tape2CurrentRecord = Record();
	tape1CurrentRecord = Record();
	isTape1SerieEnd = false;
	isTape2SerieEnd = false;
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
	outputTape->flush();
	swapTapes();
	resetTapeStates();
}

void PolyphaseSorter::handlePhaseHeaderPrint(const int& phaseNumber, bool& shouldPrintPhaseHeader, const bool& shouldPrintPhases) 
{
	if (shouldPrintPhases) {
		PrintManager::printPhaseHeader(phaseNumber, tape1->getSeriesCounter(), tape1->getDummySeriesCounter(),
			tape2->getSeriesCounter(), tape2->getDummySeriesCounter(), outputTape->getSeriesCounter(), outputTape->getDummySeriesCounter());
		printTapeState();
		//PrintManager::printPhaseHeader(phaseNumber);
		shouldPrintPhaseHeader = true;
	}
	shouldPrintPhaseHeader = false;
}

//display phasesNumber, pages written, pages read, write whats in buffer
int PolyphaseSorter::sortTapesWithFibonacci()
{
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
	outputTape->flush();
	if (shouldPrintPhases) {
		//printTapeState();
		//handlePhaseHeaderPrint(phaseNumber, shouldPrintPhaseHeader, shouldPrintPhases);
		PrintManager::printPhaseHeader(phaseNumber, tape1->getSeriesCounter(), tape1->getDummySeriesCounter(),
			tape2->getSeriesCounter(), tape2->getDummySeriesCounter(), outputTape->getSeriesCounter(), outputTape->getDummySeriesCounter());
	}

	/*PrintManager::printTapeHeader("SORTED FILE");
	outputTape->print();
	PrintManager::printTapeHeader("SORTED FILE");*/

	return phaseNumber;
}
