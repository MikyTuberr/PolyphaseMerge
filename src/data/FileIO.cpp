#include "FileIO.h"

std::streampos FileIO::read(const std::string& filename, std::vector<Record>& records, std::streampos position, size_t bytesToRead)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        throw std::runtime_error("File is not opened!\n.");
    }

    file.seekg(position);

    std::vector<std::byte> buffer(bytesToRead);
    file.read(reinterpret_cast<char*>(buffer.data()), bytesToRead);

    if (!file) {
        throw std::runtime_error("Error reading file!\n");
    }

    size_t bytesRead = file.gcount();
    size_t recordSize = sizeof(double) * 3;

    if (bytesRead < recordSize) {
        throw std::runtime_error("Not enough bytes read for at least one complete record.\n");
    }

    int recordsToRead = bytesRead / recordSize;

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

    file.close();
    return newPosition;
}

bool FileIO::write(const std::string& filename, const std::vector<Record>& records) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);

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
        file.close();
        throw;
    }

    file.close();
    return true;
}
