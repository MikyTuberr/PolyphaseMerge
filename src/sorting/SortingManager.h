#pragma once
#include "DistributionManager.h"
#include "PolyphaseSorter.h"
#include <filesystem>

class SortingManager
{
public:
	static void sortDataFromFile(const std::string& read_filename, const std::string& tape1_filename, const std::string& tape2_filename);
private:
    static void printRecords(Tape* tape) {
        bool stop = true;
        tape->resetPosition();
        while (stop) {
            std::vector<Record> records;
            stop = tape->read(records);
            for (const auto& record : records) {
                record.print();
            }
        }
        tape->resetPosition();
    }
};

