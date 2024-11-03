#pragma once
#include "../config.h"
#include "../models/Record.h"
#include <fstream>

class FileIO
{
private:
    size_t adjustBytesToRead(std::fstream& file);
    void readBuffer(std::fstream& file, std::vector<std::byte>& buffer, size_t bytesToRead);
    void populateRecords(const std::vector<std::byte>& buffer, size_t bytesRead, std::vector<Record>& records);

public:
    FileIO() : recordSize(RECORD_SIZE), blockSize(BLOCK_SIZE) {}
    ~FileIO() = default;

    bool read(std::fstream& file, std::vector<Record>& records);
    void write(std::fstream& file, const std::vector<Record>& records);

    void resetPosition();

private:
    bool isEof = false;
    std::streampos position = 0;
    std::size_t recordSize;
    std::size_t blockSize;
};

