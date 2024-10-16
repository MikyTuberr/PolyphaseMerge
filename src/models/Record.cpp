#include "Record.h"

std::vector<double> Record::getData() const
{
    return parallelogram.getPackedToVector();
}

void Record::setData(const std::vector<double> data)
{
    this->parallelogram = Parallelogram(data[0], data[1], data[2]);
}

void Record::print()
{
    parallelogram.print();
}
