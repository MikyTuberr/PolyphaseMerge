#pragma once
#include <cmath>
#include <numbers>
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>



class Parallelogram
{
public:
	Parallelogram(double aSide, double bSide, double theta)
		: content{ aSide, bSide, theta } {}

	Parallelogram()
		: content{ 0, 0, 0 } {}

	~Parallelogram() = default;

	std::vector<double> getPackedToVector() const;
	static int getContentSize();

	double area() const;
	void print() const;

	bool operator<(const Parallelogram& other) const;
	bool operator>(const Parallelogram& other) const;
	bool operator<=(const Parallelogram& other) const;
	bool operator>=(const Parallelogram& other) const;

private:
	inline struct ParallelogramContent {
		double aSide;
		double bSide;
		double theta;
	};

	ParallelogramContent content;
};

