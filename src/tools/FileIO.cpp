#include "FileIO.h"

bool FileIO::read(std::fstream& file, std::vector<Record>& records)
{
    if (!file) {
        throw std::runtime_error("File is not opened!\n.");
    }

    size_t bytesToRead = adjustBytesToRead(file);

    if (this->isEof) {
        //PrintManager::printEOFMessage();
        return false;
    }

    if (bytesToRead == 0) {
        //PrintManager::printInsufficientBytesMessage();
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

std::streampos FileIO::getPosition() const
{
    return this->position;
}

void FileIO::setPosition(std::streampos position)
{
    this->position = position;
}

void FileIO::resetPosition()
{
    this->isEof = false;
    this->position = 0;
}

bool FileIO::getEof() const
{
    return isEof;
}

void FileIO::setEof(bool eof)
{
    this->isEof = eof;
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

        if (bytesToRead == 0) {
            this->isEof = true;
            return bytesToRead;
        }

        //PrintManager::printAdjustedBytesMessage(bytesToRead, blockSize);
    }

    size_t bytesToReadMultiple = (bytesToRead / recordSize) * recordSize;

    if (bytesToReadMultiple != bytesToRead) {
        //PrintManager::printAdjustedRecordSizeMessage(bytesToRead, bytesToReadMultiple);
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
