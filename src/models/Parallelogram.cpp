#include "Parallelogram.h"

std::vector<double> Parallelogram::getPackedToVector() const
{
	return { aSide, bSide, theta };
}

double Parallelogram::area() const
{
	return aSide * bSide * sin(theta * std::numbers::pi / 180);
}

void Parallelogram::print() const
{
	std::cout << "aSide: " << aSide << ", bSide: " << bSide << ", theta: " << theta
			<< ", area: " << area() << "\n";
}

bool Parallelogram::operator<(const Parallelogram& other) const
{
	return area() < other.area();
}
