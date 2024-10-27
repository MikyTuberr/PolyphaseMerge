#pragma once
#include "../config.h"
#include "../models/Record.h"
#include <fstream>

class FileIO
{
public:
    FileIO() : recordSize(RECORD_SIZE), blockSize(BLOCK_SIZE) {}
    ~FileIO() = default;

    bool read(std::ifstream& file, std::vector<Record>& records);
    void write(std::ostream& file, const std::vector<Record>& records);

private:
    size_t adjustBytesToRead(std::ifstream& file);
    void readBuffer(std::ifstream& file, std::vector<std::byte>& buffer, size_t bytesToRead);
    void populateRecords(const std::vector<std::byte>& buffer, size_t bytesRead, std::vector<Record>& records);

    std::streampos position = 0;
    std::size_t recordSize;
    std::size_t blockSize;
};

