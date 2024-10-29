#include "DistributionManager.h"

// na zmiane rekordy
// dummy rekordy
// cleanup code
void DistributionManager::distributeSeriesWithFibonacci(std::ifstream& read_file)
{
	FileIO io;
	bool stop = true;

	while (stop) {
		std::vector<Record> records;
		stop = io.read(read_file, records);

		for (int i = 0; i < records.size(); i++) {
			if (!isPreviousRecordWritten) {
				isPreviousRecordWritten = true;
			}
			else {
				writeRecord(previousRecord);
				isSerieFinished = previousRecord > records[i];
			}
			previousRecord = records[i];

			if (isSerieFinished) {
				incrementSeriesCounter();
				manageTapeTurn();
			}
		}
	}

	writeRecord(previousRecord);
	incrementSeriesCounter();

	std::cout << "\n\nFIRST SERIE: " << tape1->getSeriesCounter() << " RECORDS: " << tape1->getRecordsCounter() << "\n";
	std::cout << "SECOND SERIE: " << tape2->getSeriesCounter() << " RECORDS: " << tape2->getRecordsCounter() << "\n";
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
	}
	else {
		tape2->write({ record });
	}
}

void DistributionManager::incrementSeriesCounter()
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
	std::pair<int, int> seriesCountToDistribute = findFibonacciPair(tape1->getSeriesCounter(), tape2->getSeriesCounter());

	if (tape1->getSeriesCounter() < seriesCountToDistribute.first) {
		isFirstTapeTurn = !isFirstTapeTurn;
	}
	else {
		isFirstTapeTurn = false;
	}
}

DistributionManager::DistributionManager(Tape* tape1, Tape* tape2)
{
	this->tape1 = tape1;
	this->tape2 = tape2;
}
