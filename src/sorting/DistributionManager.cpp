#include "DistributionManager.h"

void DistributionManager::distributeSeriesWithFibonacci()
{
	bool hasMoreRecords = true;

	while (hasMoreRecords) {
		std::vector<Record> records;
		hasMoreRecords = read_tape->read(records);

		for (const auto& record : records) {
			processRecord(record);
		}
	}

	finalizeDistribution();

	PrintManager::printTapeContentMessage("TAPE 1", tape1->getSeriesCounter(), tape1->getDummySeriesCounter(), tape1->getRecordsCounter());
	PrintManager::printTapeContentMessage("TAPE 2", tape2->getSeriesCounter(), tape2->getDummySeriesCounter(), tape2->getRecordsCounter());
}

void DistributionManager::processRecord(const Record& record)
{
	//record.print();

	if (!isFirstRecord) {
		writeRecord(previousRecord);
		isSerieEnd = previousRecord > record;
	}
	else {
		isFirstRecord = false;
	}

	if (isSerieEnd) {
		handleSerieEnd(record);
	}

	previousRecord = record;
}

void DistributionManager::handleSerieEnd(const Record& record)
{
	if (!isSerieCoalesced) {
		incrementSeriesCounters();
	}
	manageTapeTurn();
	handleSeriesCounters(record);
}

void DistributionManager::finalizeDistribution()
{
	writeRecord(previousRecord);
	if (!isSerieCoalesced) {
		incrementSeriesCounters();
	}
	addDummySeries();
}

void DistributionManager::addDummySeries()
{
	if (isFirstTapeTurn) {
		tape1->setDummySeriesCounter(numberOfSeriesForTape - tape1->getSeriesCounter());
		tape1->setSeriesCounter(numberOfSeriesForTape);
	}
	else {
		tape2->setDummySeriesCounter(numberOfSeriesForTape - tape2->getSeriesCounter());
		tape2->setSeriesCounter(numberOfSeriesForTape);
	}
}

int DistributionManager::findFibonacciNumberOfSeries(const int& firstSeriesCounter, const int& secondSeriesCounter)
{
	int a = 0, b = 1;
	int seriesCount = firstSeriesCounter + secondSeriesCounter;
	while (a + b <= seriesCount) {
		int temp = a + b;
		a = b;
		b = temp;
	}
	return b;
}

void DistributionManager::writeRecord(const Record& record)
{
	if (isFirstTapeTurn) {
		tape1->write({ record });
		tape1->setTail(record);
	}
	else {
		tape2->write({ record });
		tape2->setTail(record);
	}
}

void DistributionManager::handleSeriesCounters(const Record& record)
{
	if (isFirstTapeTurn && !tape1->isEmpty() && record > tape1->getTail()) {
		isSerieCoalesced = true;
	}
	else if (!isFirstTapeTurn && !tape2->isEmpty() && record > tape2->getTail()) {
		isSerieCoalesced = true;
	}
	else {
		isSerieCoalesced = false;
	}
}

void DistributionManager::incrementSeriesCounters()
{
	if (isFirstTapeTurn) {
		tape1->incrementSeriesCounter();
	}
	else {
		tape2->incrementSeriesCounter();
	}
}

void DistributionManager::manageTapeTurn()
{
	int tape1SeiresCounter = tape1->getSeriesCounter();
	int tape2SeriesCounter = tape2->getSeriesCounter();

	if (tape2SeriesCounter == numberOfSeriesForTape || tape1SeiresCounter == numberOfSeriesForTape) {
		numberOfSeriesForTape = findFibonacciNumberOfSeries(tape1SeiresCounter, tape2SeriesCounter);
		isFirstTapeTurn = !isFirstTapeTurn;
	}
 }

DistributionManager::DistributionManager(Tape* read_tape, Tape* tape1, Tape* tape2)
{
	this->read_tape = read_tape;
	this->tape1 = tape1;
	this->tape2 = tape2;
}
