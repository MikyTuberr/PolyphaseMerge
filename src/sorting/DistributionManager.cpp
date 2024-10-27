#include "DistributionManager.h"

// na zmiane rekordy
// dummy rekordy
// cleanup code
void DistributionManager::distributeSeriesWithFibonacci(FileIO& io, const std::vector<Record>& block, std::ofstream& file1, std::ofstream& file2)
{
	for (int i = 0; i < block.size() - 1; i++) {
		if (!isPreviousRecordWritten) {
			this->isPreviousRecordWritten = true;
			this->writeRecord(io, this->previousRecord, file1, file2);
			this->isSerieFinished = this->previousRecord >= block[i];

			i = -1;
		}
		else {
			writeRecord(io, block[i], file1, file2);
			this->isSerieFinished = block[i] >= block[i + 1];
		}

		if (this->isSerieFinished) {
			if (this->isFirstTapeTurn) {
				this->firstFileSeriesCount++;
			}
			else {
				this->secondFileSeriesCount++;
			}

			std::pair<int, int> seriesCountToDistribute = this->findFibonacciPair();

			if (this->firstFileSeriesCount < seriesCountToDistribute.first) {
				this->isFirstTapeTurn = true;
			} 
			else {
				this->isFirstTapeTurn = false;
			}
		}
		this->recordsCount++;
	}

	this->isPreviousRecordWritten = false;
	this->previousRecord = block.back();
}

std::pair<int, int> DistributionManager::findFibonacciPair()
{
	int a = 0, b = 1;
	int seriesCount = this->firstFileSeriesCount + this->secondFileSeriesCount;
	while (a + b <= seriesCount) {
		int temp = a + b;
		a = b;
		b = temp;
	}
	return std::pair<int, int>(a, b);
}

void DistributionManager::writeRecord(FileIO& io, Record record, std::ofstream& file1, std::ofstream& file2)
{
	if (this->isFirstTapeTurn) {
		io.write(file1, { record });
		this->firstFileRecordsCount++;
	}
	else {
		io.write(file2, { record });
		this->secondFileRecordsCount++;
	}
}
