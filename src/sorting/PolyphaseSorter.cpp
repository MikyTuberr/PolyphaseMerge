#include "PolyphaseSorter.h"

PolyphaseSorter::PolyphaseSorter(Tape* tape1, Tape* tape2, Tape* outputTape)
{
	this->tape1 = tape1;
	this->tape2 = tape2;
	this->outputTape = outputTape;
}

// fibonacci dystrybucja
// sprawdzanie serii
// dummy serie najpierw
void PolyphaseSorter::sortTapesWithFibonacci()
{
	bool hasMoreRecords = true;
	//while (hasMoreRecords) {
		std::vector<Record> tape1Records, tape2Records;

		hasMoreRecords = tape1->read(tape1Records);
		hasMoreRecords |= tape2->read(tape2Records);

		if (hasMoreRecords) {
			mergeSeries(tape1Records, tape2Records);

			// Zamieñ taœmy - wyjœciowa staje siê wejœciow¹, kolejna iteracja z pust¹ taœm¹
			/*if (tape1->getSeriesCounter() == 0) {
				std::swap(tape1, outputTape);
			}
			else if (tape2->getSeriesCounter() == 0) {
				std::swap(tape2, outputTape);
			}
			std::swap(tape1, tape2);
			outputTape->close();
			// przy czyszczeniu pliku possibli zwrocic uwage na counter?
			outputTape->open({ std::ios::binary, std::ios::out, std::ios::in, std::ios::trunc }); */
		}
	//}
}

void PolyphaseSorter::mergeSeries(const std::vector<Record>& tape1Records, const std::vector<Record>& tape2Records)
{
	size_t i = 0, j = 0;
	while (i < tape1Records.size() && j < tape2Records.size()) {
		Record record1 = tape1Records[i];
		Record record2 = tape2Records[j];

		if (record1 < record2) {
			isFirstTapeSerieEnd = firstTapePreviousRecord > record1;
			if (isFirstTapeSerieEnd && !isSecondTapeSerieEnd) {
				writeUntilSeriesEnd(tape2Records, j, isSecondTapeSerieEnd, secondTapePreviousRecord);
				updateCounters();
			}
			else {
				outputTape->write({ record1 });
				firstTapePreviousRecord = record1;
				i++;
			}
		}
		else {
			isSecondTapeSerieEnd = secondTapePreviousRecord > record2;
			if (isSecondTapeSerieEnd && !isFirstTapeSerieEnd) {
				writeUntilSeriesEnd(tape1Records, i, isFirstTapeSerieEnd, firstTapePreviousRecord);
				updateCounters();
			}
			else {
				outputTape->write({ record2 });
				secondTapePreviousRecord = record2;
				j++;
			}

		}
	}

	writeRemainingRecords(tape1Records, i);
	writeRemainingRecords(tape2Records, j);
}

void PolyphaseSorter::writeUntilSeriesEnd(const std::vector<Record>& records, size_t& index, bool& seriesEnd, Record& previousRecord)
{
	while (!seriesEnd && index < records.size()) {
		Record record = records[index];
		outputTape->write({ record });
		seriesEnd = previousRecord > record;
		previousRecord = record;
		index++;
	}
}

void PolyphaseSorter::writeRemainingRecords(const std::vector<Record>& records, size_t& index)
{
	while (index < records.size()) {
		outputTape->write({ records[index++] });
	}
}

void PolyphaseSorter::updateCounters()
{
	tape1->decrementSeriesCounter();
	tape2->decrementSeriesCounter();
	outputTape->incrementSeriesCounter();
}
