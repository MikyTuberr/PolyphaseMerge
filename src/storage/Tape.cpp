#include "Tape.h"

Tape::~Tape()
{
	file.close();
}

bool Tape::isOpen() const
{
	return file.is_open();
}

bool Tape::open(const std::initializer_list<std::ios::openmode> modes)
{
	std::ios::openmode combinedMode = std::ios::binary;

	for (auto mode : modes) {
		combinedMode |= mode;
	}

	file.open(filename, combinedMode);

	if (!isOpen()) {
		std::cerr << "Failed to open file: " << filename << "\n";
		return false;
	}

	return true;
}

void Tape::close()
{
	if (isOpen()) {
		file.close();
	}
}

bool Tape::write(const std::vector<Record>& records)
{
	if (!isOpen()) {
		std::cerr << "File is not opened!\n";
		return false;
	}

	for (const auto& record : records) {
		std::vector<double> data = record.getData();

		for (const auto& value : data) {
			file.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}
	}

	return true;
}

bool Tape::read(std::vector<Record>& records, std::size_t numberOfRecords)
{
	if (!isOpen()) {
		std::cerr << "File is not opened!\n";
		return false;
	}

	for (std::size_t i = 0; i < numberOfRecords; i++) {
		Record record;
		std::vector<double> data(record.getData().size());

		file.read(reinterpret_cast<char*>(data.data()), data.size() * sizeof(double));

		if (file.fail()) {
			std::cerr << "Error reading file!\n";
			return false;
		}

		record.setData(data);
		records.push_back(record);
	}

	return true;
}
