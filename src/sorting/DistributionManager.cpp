#include "DistributionManager.h"

// TODO
// sklejanie serii
// dummy rekordy
// cleanup code
void DistributionManager::distributeSeriesWithFibonacci()
{
	bool stop = true;

	while (stop) {
		std::vector<Record> records;
		stop = read_tape->read(records);

		for (int i = 0; i < records.size(); i++) {
			records[i].print();

			if (!isPreviousRecordWritten) {
				isPreviousRecordWritten = true;
			}
			else {
				writeRecord(previousRecord);
				isSerieFinished = previousRecord > records[i];
			}

			if (isSerieFinished) {
				if (!isTapeCoalesced) {
					incrementSeriesCounters();
				}
				manageTapeTurn();
				handleSeriesCounters(records[i]);
			}

			previousRecord = records[i];
		}
	}

	writeRecord(previousRecord);
	if (!isTapeCoalesced) {
		incrementSeriesCounters();
	}

	std::cout << "\n\nFIRST TAPE:\nSERIE: " << tape1->getSeriesCounter() << " RECORDS: " << tape1->getRecordsCounter() << "\n";
	std::cout << "\nSECOND TAPE\nSERIE: " << tape2->getSeriesCounter() << " RECORDS: " << tape2->getRecordsCounter() << "\n";
}

std::pair<int, int> DistributionManager::findFibonacciPair(int firstSeriesCounter, int secondSeriesCounter)
{
	int a = 0, b = 1;
	int seriesCount = firstSeriesCounter + secondSeriesCounter;
	while (a + b <= seriesCount) {
		int temp = a + b;
		a = b;
		b = temp;
	}
	return std::pair<int, int>(a, b);
}

void DistributionManager::writeRecord(Record record)
{
	if (isFirstTapeTurn) {
		tape1->write({ record });
		tape1Tail = record;
		if (!isTape1Written) {
			isTape1Written = true;
		}
	}
	else {
		tape2->write({ record });
		tape2Tail = record;
		if (!isTape2Written) {
			isTape2Written = true;
		}
	}
}

void DistributionManager::handleSeriesCounters(Record record)
{
	if (isFirstTapeTurn && isTape1Written && record > tape1Tail) {
		isTapeCoalesced = true;
		//std::cout << "DECREMENT 1: " << tape1->getSeriesCounter() << "\n";
	}
	else if (!isFirstTapeTurn && isTape2Written && record > tape2Tail) {
		isTapeCoalesced = true;
		//std::cout << "DECREMENT 2: " << tape2->getSeriesCounter() << "\n";
	}
	else {
		isTapeCoalesced = false;
	}
}

void DistributionManager::incrementSeriesCounters()
{
	if (isFirstTapeTurn) {
		tape1->incrementSeriesCounter();
		//std::cout << "INCREMENT 1: " << tape1->getSeriesCounter() << "\n";
	}
	else {
		tape2->incrementSeriesCounter();
		//std::cout << "INCREMENT 2: " << tape2->getSeriesCounter() << "\n";
	}
}

void DistributionManager::manageTapeTurn()
{
	std::pair<int, int> seriesCountToDistribute = findFibonacciPair(tape1->getSeriesCounter(), tape2->getSeriesCounter());

	if (tape1->getSeriesCounter() < seriesCountToDistribute.first) {
		isFirstTapeTurn = !isFirstTapeTurn;
	}
	else {
		isFirstTapeTurn = false;
	}
}

DistributionManager::DistributionManager(Tape* read_tape, Tape* tape1, Tape* tape2)
{
	this->read_tape = read_tape;
	this->tape1 = tape1;
	this->tape2 = tape2;
}
