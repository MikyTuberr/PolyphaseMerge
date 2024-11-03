#include "Tape.h"

Tape::Tape(const std::string& filename) 
    : filename(filename)
{
}

Tape::~Tape()
{
    if (file.is_open()) {
        file.close();
    }
}

bool Tape::isOpen() const
{
	return file.is_open();
}

bool Tape::isEmpty() const
{
	return _isEmpty;
}

bool Tape::open(const std::initializer_list<std::ios::openmode> modes)
{
	std::ios::openmode combinedMode = std::ios::binary;
	for (auto mode : modes) {
		combinedMode |= mode;
	}
	file.open(filename, combinedMode);
	if (!isOpen()) {
		std::cerr << "Failed to open file: " << filename << "\n";
		return false;
	}
	return true;
}

void Tape::close()
{
	if (isOpen()) {
		file.close();
	}
}

bool Tape::write(const std::vector<Record>& records)
{
	if (!isOpen()) {
		std::cerr << "File is not opened!\n";
		return false;
	}
	if (_isEmpty) {
		_isEmpty = false;
	}
	io.write(file, records);
	incrementRecordsCounter();
	return true;
}

bool Tape::read(std::vector<Record>& records)
{
	if (!isOpen()) {
		std::cerr << "File is not opened!\n";
		return false;
	}

	return io.read(file, records);
}

void Tape::print()
{
	if (!isOpen()) {
		std::cerr << "File is not opened!\n";
		return;
	}

	bool stop = true;
	std::streampos tmpPos = io.getPosition();
	bool tmpEof = io.getEof();
	io.resetPosition();
	while (stop) {
		std::vector<Record> records;
		stop = read(records);
		for (const auto& record : records) {
			record.print();
		}
	}
	io.setEof(tmpEof);
	io.setPosition(tmpPos);
}

void Tape::incrementSeriesCounter()
{
	seriesCounter++;
}

bool Tape::decrementSeriesCounter()
{
	if (seriesCounter > 0) {
		seriesCounter--;
		return true;
	}
	else {
		return false;
	}
}

bool Tape::decrementDummySeriesCounter()
{
	if (dummySeriesCounter > 0) {
		dummySeriesCounter--;
		return true;
	}
	else {
		return false;
	}
}

void Tape::incrementRecordsCounter()
{
	recordsCounter++;
}

bool Tape::decrementRecordsCounter()
{
	if (recordsCounter > 0) {
		recordsCounter--;
		return true;
	}
	else {
		return false;
	}
}

int Tape::getSeriesCounter() const
{
	return seriesCounter;
}

int Tape::getDummySeriesCounter() const
{
	return dummySeriesCounter;
}

int Tape::getRecordsCounter() const
{
	return recordsCounter;
}

Record Tape::getTail() const
{
	return tail;
}

void Tape::setSeriesCounter(int seriesCount)
{
	seriesCounter = seriesCount;
}

void Tape::setDummySeriesCounter(int dummySeriesCount)
{
	dummySeriesCounter = dummySeriesCount;
}

void Tape::setTail(Record record)
{
	tail = record;
}

void Tape::resetPosition()
{
	io.resetPosition();
}
