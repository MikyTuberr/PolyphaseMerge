#include "config.h"
#include "tools/RecordFactory.h"
#include "tools/FileIO.h"
#include "sorting/SortingManager.h"
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

    std::cout << "\nStarting block reading...\n\n";
    try {
        SortingManager manager;
        manager.sortDataFromFile(read_file);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what();
    }

    std::cout << "\nFinishing block reading...\n";

    read_file.close();
    std::filesystem::remove("src/data/tape1.bin");
    std::filesystem::remove("src/data/tape2.bin");
    return 0;
}