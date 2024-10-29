#pragma once
#include "DistributionManager.h"

class SortingManager
{
public:
	void sortDataFromFile(const std::string& read_filename, const std::string& tape1_filename, const std::string& tape2_filename);

    void printRecords(const std::string& binFilename) {
        FileIO io;
        std::fstream bin(binFilename, std::ios::binary | std::ios::in);
        bool stop = true;
        while (stop) {
            std::vector<Record> records;
            stop = io.read(bin, records);
            for (const auto& record : records) {
                record.print();
            }
        }
        bin.close();
    }
};

