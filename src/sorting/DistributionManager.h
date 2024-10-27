#pragma once
#include <utility>
#include <vector>
#include "../tools/FileIO.h"
#include "../models/Record.h"

class DistributionManager 
{
private:
	std::pair<int, int> findFibonacciPair();
	void writeRecord(FileIO& io, Record record, std::ofstream& file1, std::ofstream& file2);
public:
	DistributionManager() = default;
	~DistributionManager() {
		std::cout << "\n\nFIRST SERIE: " << firstFileSeriesCount << " RECORDS: " << firstFileRecordsCount << "\n";
		std::cout << "SECOND SERIE: " << secondFileSeriesCount << " RECORDS: " << secondFileRecordsCount << "\n";
	}

	void distributeSeriesWithFibonacci(FileIO& io, const std::vector<Record>& block, std::ofstream& file1, std::ofstream& file2);
private:
	Record previousRecord = Record(DBL_MAX, DBL_MAX, DBL_MAX);
	int firstFileRecordsCount = 0;
	int secondFileRecordsCount = 0;
	int firstFileSeriesCount = 0;
	int secondFileSeriesCount = 0;
	bool isSerieFinished = true;
	bool isFirstTapeTurn = true;
	bool isPreviousRecordWritten = true;
	int recordsCount = 0;
};