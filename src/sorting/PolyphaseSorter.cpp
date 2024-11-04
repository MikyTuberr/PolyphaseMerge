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
	outputTape->write({ record });
	tape->decrementRecordsCounter();
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
	tape2->decrementSeriesCounter();
	outputTape->incrementSeriesCounter();
	numberOfSeriesToMerge--;
	isTape1SerieEnd = false;
	isTape2SerieEnd = false;
}

// TODO 
// dummy series
// tests!!!!!!!!!!!!
// cleanup code!!!!
void PolyphaseSorter::processBlocks(int& numberOfSeriesToMerge)
{
	bool read1Tape = true;
	bool read2Tape = true;

	while (numberOfSeriesToMerge > 0 && (read1Tape || read2Tape)) {

		// read
		if (read1Tape && tape1Index == tape1Records.size()) {
			tape1Records = std::vector<Record>();
			tape1Index = 0;
			read1Tape = tape1->read(tape1Records);
		}

		if (read2Tape && tape2Index == tape2Records.size()) {
			tape2Records = std::vector<Record>();
			tape2Index = 0;
			read2Tape = tape2->read(tape2Records);
		}

		// check serie from previous block
		if (!read1Tape) {
			isTape1SerieEnd = true;
			tape1CurrentRecord = Record();
		}
		else if (tape1Index == 0 && tape1CurrentRecord != tape1Records[tape1Index]) {
			isTape1SerieEnd = tape1CurrentRecord > tape1Records[tape1Index];
			tape1CurrentRecord = tape1Records[tape1Index];
		}
		else {
			tape1CurrentRecord = tape1Records[tape1Index];
		}

		if (!read2Tape) {
			isTape2SerieEnd = true;
			tape2CurrentRecord = Record();
		}
		else if (tape2Index == 0 && tape2CurrentRecord != tape2Records[tape2Index]) {
			isTape2SerieEnd = tape2CurrentRecord > tape2Records[tape2Index];
			tape2CurrentRecord = tape2Records[tape2Index];
		}
		else {
			tape2CurrentRecord = tape2Records[tape2Index];
		}
		

		// serie sort alg
		if (!isTape1SerieEnd && !isTape2SerieEnd) {
			compareRecords(tape1Index, tape2Index);
		}
		else if (isTape1SerieEnd && !isTape2SerieEnd) {
			writeToOutputTape(tape2, tape2Records, tape2CurrentRecord, tape2Index, isTape2SerieEnd);
		}
		else if (!isTape1SerieEnd && isTape2SerieEnd) {
			writeToOutputTape(tape1, tape1Records, tape1CurrentRecord, tape1Index, isTape1SerieEnd);
		}
		else if (isTape1SerieEnd && isTape2SerieEnd) {
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
	//std::swap(tape1, tape2);
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

void PolyphaseSorter::sortTapesWithFibonacci()
{
	int phaseNumber = 1;
	bool shouldPrintPhaseHeader = true;

	while (true) {
		int numberOfSeriesToMerge = getNumberOfSeriesToMerge();

		if (shouldPrintPhaseHeader) {
			PrintManager::printPhaseHeader(phaseNumber);
			shouldPrintPhaseHeader = false;
		}

		processBlocks(numberOfSeriesToMerge);
	

		if (tape1->getSeriesCounter() == 0 && tape2->getSeriesCounter() == 0) {
			break;
		}
		
		if (numberOfSeriesToMerge == 0) {

			if (PrintManager::promptUserDisplayDecision()) {
				PrintManager::printTapeHeader("TAPE 1");
				tape1->print();
				PrintManager::printTapeHeader("TAPE 2");
				tape2->print();
				PrintManager::printTapeHeader("TAPE 3 (OUTPUT)");
				outputTape->print();
			}

			PrintManager::printPhaseHeader(phaseNumber);
			shouldPrintPhaseHeader = true;

			swapTapes();
			resetTapeStates();
			phaseNumber++;
		}
	}

	PrintManager::printPhaseHeader(phaseNumber);
	PrintManager::printTapeHeader("SORTED TAPE");
	outputTape->print();
}
