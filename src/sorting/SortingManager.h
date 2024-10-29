#pragma once
#include "DistributionManager.h"

class SortingManager
{
public:
	void sortDataFromFile(std::ifstream& read_file);
    void printRecords(const std::string& binFilename) {
        FileIO io;
        std::ifstream bin(binFilename, std::ios::binary);
        bool stop = true;
        while (stop) {
            std::vector<Record> records;
            stop = io.read(bin, records);
            for (const auto& record : records) {
                record.print();
            }
        }
    }
};

