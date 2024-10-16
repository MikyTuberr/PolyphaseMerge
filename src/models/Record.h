#pragma once
#include "Parallelogram.h"

class Record
{
public:
	Record(const Parallelogram& parallelogram) : parallelogram(parallelogram) {};
	Record() : parallelogram(Parallelogram(0, 0, 0)) {}
	~Record() = default;

	std::vector<double> getData() const;
	void setData(const std::vector<double> data);
	void print();
private:
	Parallelogram parallelogram;
};

