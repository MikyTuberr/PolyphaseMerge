#pragma once
#include "PrintManager.h"
#include "../config.h"
#include "../models/Record.h"
#include <fstream>

class FileIO
{
private:
    size_t adjustBytesToRead(std::fstream& file);
    void readBuffer(std::fstream& file, std::vector<std::byte>& buffer, size_t bytesToRead);
    void populateRecords(const std::vector<std::byte>& buffer, size_t bytesRead, std::vector<Record>& records);

    void writeBlock(const bool& countPage);
    bool readBlock(std::vector<Record>& records, const bool& countPage);

public:
    FileIO() : blockSize(BLOCK_SIZE), recordSize(RECORD_SIZE) {}
    FileIO(const std::string& filename) : blockSize(BLOCK_SIZE), recordSize(RECORD_SIZE), filename(filename), 
        file(filename) {}
    FileIO(FileIO&& other) noexcept;
    FileIO& operator=(FileIO&& other) noexcept;
    ~FileIO();

    bool isEmpty() const;

    void printFile();
    bool open(const std::initializer_list<std::ios::openmode> modes);
    void close();

    bool readRecord(Record& record, const bool& countPage);
    void writeRecord(const Record& record, const bool& countPage);
    void flush(const bool& countPage);
    size_t getRecordsToWriteSize() const;

    void resetPosition();
    
    size_t getPagesWritten() const;
    size_t getPagesRead() const;
private:
    bool isEof = false;
    bool _isEmpty = true;
    size_t pagesRead = 0, pagesWritten = 0;
    std::fstream file;
    std::string filename;
    std::streampos position = 0;
    std::size_t recordSize, blockSize;
    std::vector<Record> recordsToWrite, recordsRead;
    size_t readIndex = 0;
};

