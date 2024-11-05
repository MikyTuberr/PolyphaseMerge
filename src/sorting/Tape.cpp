#include "Tape.h"

Tape::Tape(const std::string& filename) 
{
	io = FileIO(filename);
}

bool Tape::isEmpty() const
{
	return io.isEmpty();
}

bool Tape::open(const std::initializer_list<std::ios::openmode> modes)
{
	return io.open(modes);
}

void Tape::close()
{
	io.close();
}

void Tape::writeRecord(const Record& record)
{
	io.writeRecord(record);
}

void Tape::flush()
{
	io.flush();
}

bool Tape::read(std::vector<Record>& records)
{
	return io.read(records, true);
}

void Tape::print()
{
	io.printFile();
}

void Tape::printContent()
{
	PrintManager::printTapeContentMessage(seriesCounter, dummySeriesCounter);
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

int Tape::getSeriesCounter() const
{
	return seriesCounter;
}

int Tape::getDummySeriesCounter() const
{
	return dummySeriesCounter;
}

Record Tape::getTail() const
{
	return tail;
}

void Tape::setSeriesCounter(const int seriesCount)
{
	seriesCounter = seriesCount;
}

void Tape::setDummySeriesCounter(const int dummySeriesCount)
{
	dummySeriesCounter = dummySeriesCount;
}

void Tape::setTail(const Record record)
{
	tail = record;
}


void Tape::resetPosition()
{
	io.resetPosition();
}

size_t Tape::getPagesWritten() const
{
	return io.getPagesWritten();
}

size_t Tape::getPagesRead() const
{
	return io.getPagesRead();
}
