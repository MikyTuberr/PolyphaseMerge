#include "storage/Tape.h"
#include "data/RecordFactory.h"
#include "data/FileIO.h"

int main() {
    std::string filename = "src/data.bin";

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
            RecordFactory::createRandomRecords(filename, numRecords);
        }
        else if (choice == 2) {
            RecordFactory::createManualRecords(filename, numRecords);
        }
        else {
            std::cout << "Invalid option!\n";
            return 1;
        }
    }

    FileIO fileIO;
    std::vector<Record> records;

    try {
        std::streampos position = 0;
        size_t recordsToRead = 5000;
        position = fileIO.read(filename, records, position, recordsToRead);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    for (const auto& record : records) {
        record.print();
    }

    return 0;
}