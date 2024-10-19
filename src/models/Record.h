#pragma once
#include "Parallelogram.h"

class Record
{
public:
	Record() : parallelogram(Parallelogram()) {}
	Record(const Parallelogram& parallelogram) : parallelogram(parallelogram) {}
	Record(const std::vector<double>& data) : parallelogram(data[0], data[1], data[2]) {}
	Record(double a, double b, double theta) : parallelogram(a, b, theta) {}
	~Record() = default;

	std::vector<double> getData() const;
	void setData(const std::vector<double> data);
	void print() const;
private:
	Parallelogram parallelogram;
};

