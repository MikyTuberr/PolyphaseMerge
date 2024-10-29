#include "tools/RecordFactory.h"
#include "sorting/SortingManager.h"
#include <filesystem>

int main() {
    std::string read_tape_filename = "src/data/data.bin";

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
            RecordFactory::createRandomRecords(read_tape_filename, numRecords);
        }
        else if (choice == 2) {
            RecordFactory::createManualRecords(read_tape_filename, numRecords);
        }
        else {
            std::cout << "Invalid option!\n";
            return 1;
        }
    }

    std::string tape1_filename = "src/data/tape1.bin";
    std::string tape2_filename = "src/data/tape2.bin";

    std::cout << "\nStarting block reading...\n\n";
    try {
        SortingManager manager;
        manager.sortDataFromFile(read_tape_filename, tape1_filename, tape2_filename);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what();
    }

    std::cout << "\nFinishing block reading...\n";

    std::filesystem::remove(tape1_filename);
    std::filesystem::remove(tape2_filename);
    return 0;
}