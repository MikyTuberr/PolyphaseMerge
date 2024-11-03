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

bool PolyphaseSorter::readBlocksFromTapes()
{
	bool keepTheRead = true;
	bool hasMoreRecords = false;

	if (readFromTape1) {
		hasMoreRecords |= tape1->read(tape1Records);
	}
	if (readFromTape2) {
		hasMoreRecords |= tape2->read(tape2Records);
	}

	if (readFromTape1 && readFromTape2) {
		return hasMoreRecords;
	}
	
	return keepTheRead;
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
	outputTape->incrementRecordsCounter();
	record.print();
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

void PolyphaseSorter::handleTapeEnd(Tape* tape, std::vector<Record>& records, size_t& index, Record& currentRecord, 
	bool& serieEndFlag, int& numberOfSeriesToMerge)
{
	while (index < records.size() && !serieEndFlag) {
		currentRecord = records[index];
		writeToOutputTape(tape, records, currentRecord, index, serieEndFlag);
	}
	records.erase(records.begin(), records.begin() + index);
	index = 0;
	if (serieEndFlag) {
		handleSerieMergeEnd(numberOfSeriesToMerge);
	}
}

void PolyphaseSorter::processBlocks(int& numberOfSeriesToMerge)
{
	readFromTape1 = true;
	readFromTape2 = true;

	while (numberOfSeriesToMerge > 0 && tape1Index < tape1Records.size() && tape2Index < tape2Records.size()) {

		if (tape1Index == 0 && tape1CurrentRecord != tape1Records[tape1Index]) {
			isTape1SerieEnd = tape1CurrentRecord > tape1Records[tape1Index];
		}

		if (tape2Index == 0 && tape2CurrentRecord != tape2Records[tape2Index]) {
			isTape2SerieEnd = tape2CurrentRecord > tape2Records[tape2Index];
		}

		tape1CurrentRecord = tape1Records[tape1Index];
		tape2CurrentRecord = tape2Records[tape2Index];

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

	if(tape1Index < tape1Records.size() && tape2Records.size() > 0) {
		readFromTape1 = false;
		tape2Records = std::vector<Record>();
		tape2Index = 0;
	}
	else if (tape2Index < tape2Records.size() && tape1Records.size() > 0) {
		readFromTape2 = false;
		tape1Records = std::vector<Record>();
		tape1Index = 0;
	}
	else if (tape1Records.size() > 0 && tape2Records.size() == 0) {
		handleTapeEnd(tape1, tape1Records, tape1Index, tape1CurrentRecord, isTape1SerieEnd, numberOfSeriesToMerge);
	}
	else if (tape2Records.size() > 0 && tape1Records.size() == 0) {
		handleTapeEnd(tape2, tape2Records, tape2Index, tape2CurrentRecord, isTape2SerieEnd, numberOfSeriesToMerge);
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
	std::swap(tape1, tape2);
	outputTape->close();
	outputTape->resetPosition();
	outputTape->open({ std::ios::binary, std::ios::out, std::ios::in, std::ios::trunc });
}


void PolyphaseSorter::handleRemainingRecords()
{
	if (tape1Records.size() > 0) {
		while (tape1Index < tape1Records.size()) {
			tape1CurrentRecord = tape1Records[tape1Index];
			writeToOutputTape(tape1, tape1Records, tape1CurrentRecord, tape1Index, isTape1SerieEnd);
		}
	} 
	else if (tape2Records.size() > 0) {
		while (tape2Index < tape2Records.size()) {
			tape2CurrentRecord = tape2Records[tape2Index];
			writeToOutputTape(tape2, tape2Records, tape2CurrentRecord, tape2Index, isTape2SerieEnd);
		}
	}
}

void PolyphaseSorter::sortTapesWithFibonacci()
{
	int numberOfSeriesToMerge = 0;
	int phaseNumber = 1;
	bool print = true;
	bool read = true;
	while (read) {
		read = readBlocksFromTapes();
		numberOfSeriesToMerge = getNumberOfSeriesToMerge();

		if (print) {
			std::cout << "\n\n================================PHASE " << phaseNumber << "================================\n\n";
			print = false;
		}

		if (numberOfSeriesToMerge > 0) {
			processBlocks(numberOfSeriesToMerge);
		}

		if (tape1->getSeriesCounter() == 0 && tape2->getSeriesCounter() == 0) {
			break;
		}
		
		if (numberOfSeriesToMerge == 0) {
			std::cout << "\n\n================================PHASE " << phaseNumber << "================================\n\n";
			print = true;
			swapTapes();
			phaseNumber++;
		}
	}

	std::cout << "\n\n================================PHASE " << phaseNumber << "================================\n\n";

	/*if (numberOfSeriesToMerge > 0) {
		handleRemainingRecords();
	}*/
}
