#include "Parallelogram.h"

std::vector<double> Parallelogram::getPackedToVector() const
{
	return { content.aSide, content.bSide, content.theta };
}

int Parallelogram::getContentSize()
{
	return sizeof(ParallelogramContent);
}

double Parallelogram::area() const
{
	return content.aSide * content.bSide * sin(content.theta * std::numbers::pi / 180);
}

void Parallelogram::print() const
{
	std::cout << std::fixed << std::setprecision(2) << 
		"aSide: " << content.aSide << 
		", bSide: " << content.bSide << 
		", theta: " << content.theta << 
		", area: " << area() << "\n";
}

bool Parallelogram::operator<(const Parallelogram& other) const
{
	return area() < other.area();
}

bool Parallelogram::operator>(const Parallelogram& other) const
{
	return area() > other.area();
}

bool Parallelogram::operator<=(const Parallelogram& other) const
{
	return area() <= other.area();
}

bool Parallelogram::operator>=(const Parallelogram& other) const
{
	return area() >= other.area();
}

bool Parallelogram::operator!=(const Parallelogram& other) const
{
	return area() != other.area();
}
