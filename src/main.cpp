#include "config.h"
#include "tools/RecordFactory.h"
#include "tools/FileIO.h"
#include "sorting/DistributionManager.h"
#include <filesystem>

int main() {
    std::string filename = "src/data/data.bin";
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    FileIO io;

    int choice;
    std::cout << "Select an option:\n";
    std::cout << "1. Generate random records and save to file\n";
    std::cout << "2. Enter records manually and save to file\n";
    std::cout << "3. Skip\n";
    std::cin >> choice;

    if (choice != 3) {
        int numRecords;
        std::cout << "Enter the number of records: ";
        std::cin >> numRecords;

        if (choice == 1) {
            RecordFactory::createRandomRecords(io, file, numRecords);
        }
        else if (choice == 2) {
            RecordFactory::createManualRecords(io, file, numRecords);
        }
        else {
            std::cout << "Invalid option!\n";
            return 1;
        }
    }

    file.close();

    std::ifstream read_file(filename, std::ios::binary);
    bool stop = true;
    int blocks_completed = 0;

    std::cout << "\nStarting block reading...\n\n";
    try {
        DistributionManager manager;
        std::ofstream tape1("src/data/tape1.bin", std::ios::binary | std::ios::app);
        std::ofstream tape2("src/data/tape2.bin", std::ios::binary | std::ios::app);
        while (stop) {
            std::vector<Record> records;
            stop = io.read(read_file, records);
            blocks_completed++;

            if (records.size() > 0) manager.distributeSeriesWithFibonacci(io, records, tape1, tape2);
        }
        tape1.close();
        tape2.close();

        stop = true;
        int firstTapeSeries = 0, secondTapeSeries = 0;

        FileIO io1, io2;
        std::ifstream tape11("src/data/tape1.bin", std::ios::binary);
        std::ifstream tape22("src/data/tape2.bin", std::ios::binary);
        while (stop) {
            /*std::cout << "=====================\n";
            std::cout << "FIRST TAPE\n";
            std::cout << "=====================\n";*/
            std::vector<Record> records;
            stop = io1.read(tape11, records);
            for (const auto& record : records) {
                //record.print();
                secondTapeSeries++;
            }
        }

        stop = true;
        while (stop) {
            /*std::cout << "=====================\n";
            std::cout << "SECOND TAPE\n";
            std::cout << "=====================\n";*/
            std::vector<Record> records;
            stop = io2.read(tape22, records);
            for (const auto& record : records) {
                //record.print();
                firstTapeSeries++;
            }
        }
        tape11.close();
        tape22.close();
        std::cout << "\n FIRST: " << firstTapeSeries << "\n";
        std::cout << "SECOND: " << secondTapeSeries << "\n";
        std::cout << "RECORD SIZE: " << RECORD_SIZE << "\n";
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what();
        std::cerr << "Completed blocks: " << blocks_completed << "\n\n";
    }

    std::cout << "\nFinishing block reading...\n";
    
    read_file.close();

    return 0;
}