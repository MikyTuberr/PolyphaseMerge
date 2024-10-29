#include "FileIO.h"

bool FileIO::read(std::fstream& file, std::vector<Record>& records)
{
    if (!file) {
        throw std::runtime_error("File is not opened!\n.");
    }

    size_t bytesToRead = adjustBytesToRead(file);

    if (bytesToRead == 0) {
        std::cout << "~~~ Provided number of bytes to read is smaller than one complete record size. ~~~\n";
        return false;
    }

    std::vector<std::byte> buffer(bytesToRead);
    readBuffer(file, buffer, bytesToRead);

    size_t bytesRead = buffer.size();
    populateRecords(buffer, bytesRead, records);

    std::streamoff offset = (bytesRead / recordSize) * recordSize;
    position += offset;

    return true;
}

void FileIO::write(std::fstream& file, const std::vector<Record>& records) {

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

size_t FileIO::adjustBytesToRead(std::fstream& file)
{
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(position);
    size_t bytesToRead = blockSize;

    if (static_cast<std::streamoff>(position) + static_cast<std::streamoff>(bytesToRead) >
        static_cast<std::streamoff>(fileSize))
    {
        bytesToRead = static_cast<size_t>(fileSize - position);
        std::cout << "~~~ End of file detected, adjusting bytesToRead to " << bytesToRead << ". ~~~\n";
    }

    size_t bytesToReadMultiple = (bytesToRead / recordSize) * recordSize;

    if (bytesToReadMultiple != bytesToRead) {
        std::cout << "~~~ Provided number of bytes: " << bytesToRead << " to read is not a multiple of the record size, changing to : "
            << bytesToReadMultiple << ". ~~~\n";
        bytesToRead = bytesToReadMultiple;
    }

    return bytesToRead;
}

void FileIO::readBuffer(std::fstream& file, std::vector<std::byte>& buffer, size_t bytesToRead)
{
    file.read(reinterpret_cast<char*>(buffer.data()), bytesToRead);
    if (!file) {
        throw std::runtime_error("Error reading file!\n");
    }
}

void FileIO::populateRecords(const std::vector<std::byte>& buffer, size_t bytesRead, std::vector<Record>& records)
{
    size_t recordsToRead = bytesRead / recordSize;

    for (size_t i = 0; i < recordsToRead; ++i) {
        std::vector<double> values(3);
        std::memcpy(values.data(), buffer.data() + (i * recordSize), recordSize);
        records.push_back(Record(values));
    }
}
