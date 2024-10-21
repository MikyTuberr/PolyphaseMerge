#include "config.h"
#include "storage/Tape.h"
#include "tools/RecordFactory.h"
#include "tools/FileIO.h"
#include <filesystem>

int main() {
    std::string filename = "src/data/data.bin";

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

    int blocks = 100;
    int blocks_completed = 0;
    std::ifstream file(filename, std::ios::binary);
    std::streampos position = 0;

    std::cout << "\nStarting block reading...\n\n";
    std::vector<Tape> tapes;

    try {
        for (int i = 0; i < blocks; i++) {
            std::cout << "===============" << "\n";
            std::cout << "Block number: " << i << "\n";
            std::cout << "===============" << "\n";
            std::vector<Record> records;
            position = FileIO::read(file, records, position, BLOCK_SIZE);
            Tape tape(i);
            tape.write(records);
            tapes.push_back(tape);

            blocks_completed++;

            for (const auto& record : records) {
                record.print();
            }
        }

    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what();
        std::cerr << "Completed blocks: " << blocks_completed << "\n\n";
    }

    std::cout << "\nFinishing block reading...\n";
    
    file.close();

    for (const auto& tape : tapes) {
        try {
            tape.print();
            std::filesystem::remove(tape.getFilename());
        }
        catch (const std::runtime_error& e) {
            std::cerr << e.what();
            return 0;
        }
    }

    return 0;
}