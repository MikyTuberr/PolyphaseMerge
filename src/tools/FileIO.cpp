#include "FileIO.h"

std::streampos FileIO::read(std::ifstream& file, std::vector<Record>& records, std::streampos position,
    size_t bytesToRead)
{

    if (!file) {
        throw std::runtime_error("File is not opened!\n.");
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(position);

    if (static_cast<std::streamoff>(position) + static_cast<std::streamoff>(bytesToRead) > 
        static_cast<std::streamoff>(fileSize)) 
    {
        bytesToRead = static_cast<size_t>(fileSize - position);

        bytesToRead = (bytesToRead / RECORD_SIZE) * RECORD_SIZE;

        if (bytesToRead == 0) {;
            std::cout << "Provided number of bytes to read is smaller than at least one complete record (24).\n";
            return position;
        }
        std::cout << "Provided number of bytes to read is not multiplicity of one record size (24), changing to: " << bytesToRead << "\n";
    }

    std::vector<std::byte> buffer(bytesToRead);
    file.read(reinterpret_cast<char*>(buffer.data()), bytesToRead);

    if (!file) {
        throw std::runtime_error("Error reading file!\n");
    }

    size_t bytesRead = file.gcount();
    size_t recordSize = RECORD_SIZE;
    size_t recordsToRead = bytesRead / recordSize;

    for (size_t i = 0; i < recordsToRead; ++i) {
        std::vector<double> values(3);
        for (int j = 0; j < 3; ++j) {
            std::memcpy(&values[j], buffer.data() + (i * recordSize) + (j * sizeof(double)), sizeof(double));
        }

        Record record(values);
        records.push_back(record);
    }

    std::streamoff offset = recordsToRead * recordSize;
    std::streampos newPosition = position + offset; 

    return newPosition;
}

void FileIO::write(std::ostream& file, const std::vector<Record>& records) {

    if (!file) {
        throw std::runtime_error("File is not opened!\n");
    }

    try {
        for (const auto& record : records) {
            std::vector<double> data = record.getData();

            for (const auto& value : data) {
                file.write(reinterpret_cast<const char*>(&value), sizeof(value));

                if (!file) {
                    throw std::runtime_error("Error writing to file!\n");
                }
            }
        }
    }
    catch (const std::exception& e) {
        throw e;
    }
}
