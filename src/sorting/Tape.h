#pragma once
#include <fstream>
#include <vector>
#include "../models/Record.h"
#include "../tools/FileIO.h"

class Tape
{
public:
	Tape() = default;
	Tape(const std::string& filename);
	~Tape();

	bool isOpen() const;
	bool isEmpty() const;

	bool open(const std::initializer_list<std::ios::openmode> modes);
	void close();
	bool write(const std::vector<Record>& records);
	bool read(std::vector<Record>& records);
	void print();
	void printContent();

	void incrementSeriesCounter();
	bool decrementSeriesCounter();
	bool decrementDummySeriesCounter();

	int getSeriesCounter() const;
	int getDummySeriesCounter() const;
	Record getTail() const;

	void setSeriesCounter(const int seriesCount);
	void setDummySeriesCounter(const int dummySeriesCount);
	void setTail(const Record record);
	void resetPosition();

private:
	FileIO io;
	std::string filename;
	std::fstream file;
	Record tail;
	bool _isEmpty = true;
	int seriesCounter = 0;
	int dummySeriesCounter = 0;
};

