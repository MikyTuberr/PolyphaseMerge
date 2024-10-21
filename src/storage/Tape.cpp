#include "Tape.h"

std::string Tape::getFilename() const
{
	return this->filename;
}

void Tape::write(const std::vector<Record>& records)
{
	std::ofstream file(filename, std::ios::binary | std::ios::app);

	FileIO::write(file, records);

	file.close();
}

std::streampos Tape::read(std::vector<Record>& records, std::streampos position, size_t bytesToRead)
{
	std::ifstream file(filename, std::ios::binary);

	position = FileIO::read(file, records, position, bytesToRead);

	file.close();
	return position;
}

void Tape::print() const
{
	std::cout << "===============" << "\n";
	std::cout << "Tape filename: " << filename << "\n";
	std::cout << "===============" << "\n";
	std::vector<Record> records;
	std::ifstream file(filename, std::ios::binary);
	std::streampos position = 0;
	FileIO::read(file, records, position, BLOCK_SIZE);
	for (const auto& record : records) {
		record.print();
	}

}
