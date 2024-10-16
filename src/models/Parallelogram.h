#pragma once
#include <cmath>
#include <numbers>
#include <string>
#include <iostream>
#include <vector>

class Parallelogram
{
public:
	Parallelogram(double aSide, double bSide, double theta) : aSide(aSide), bSide(bSide), theta(theta) {};
	~Parallelogram() = default;

	std::vector<double> getPackedToVector() const;
	double area() const;
	void print() const;

	bool operator<(const Parallelogram& other) const;

private:
	double aSide;
	double bSide;
	double theta;
};

