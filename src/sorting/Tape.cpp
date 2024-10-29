#include "Tape.h"

Tape::Tape(const std::string& filename) 
    : filename(filename),
    recordsCounter(0),
    seriesCounter(0)
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

	return io.read(file, records);;
}

void Tape::incrementSeriesCounter()
{
	seriesCounter++;
}

void Tape::incrementRecordsCounter()
{
	recordsCounter++;
}

int Tape::getSeriesCounter() const
{
	return seriesCounter;
}

int Tape::getRecordsCounter() const
{
	return recordsCounter;
}
