#include "Record.h"

int Record::getDataSize()
{
    return Parallelogram::getContentSize();
}

std::vector<double> Record::getData() const
{
    return parallelogram.getPackedToVector();
}

Parallelogram Record::getParallelogram() const
{
    return parallelogram;
}

void Record::setData(const std::vector<double> data)
{
    parallelogram = Parallelogram(data[0], data[1], data[2]);
}

void Record::print() const
{
    parallelogram.print();
}

bool Record::operator<(const Record& other) const
{
    return parallelogram < other.getParallelogram();
}

bool Record::operator>(const Record& other) const
{
    return parallelogram > other.getParallelogram();
}

bool Record::operator<=(const Record& other) const
{
    return parallelogram <= other.getParallelogram();
}

bool Record::operator>=(const Record& other) const
{
    return parallelogram >= other.getParallelogram();
}
