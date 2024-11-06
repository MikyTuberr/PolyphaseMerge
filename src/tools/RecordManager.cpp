#include "RecordManager.h"

void RecordManager::displayOptions()
{
    std::cout << "Options:\n";
    std::cout << "1. Generate random records and save to file\n";
    std::cout << "2. Enter records manually and save to file\n";
    std::cout << "3. Sort the file\n";
}

int RecordManager::handleOptionInput()
{
    int option = 0;
    std::cout << "Select the options: ";
    std::cin >> option;
    return option;
}

int RecordManager::handleRecordNumberInput()
{
    int numRecords;
    std::cout << "Enter the number of records: ";
    std::cin >> numRecords;
    return numRecords;
}

void RecordManager::handleRecordCreation(const std::string& filename)
{
    int option = 0;

    while (option != 3) {
        displayOptions();
        option = handleOptionInput();
        if (option == 1) {
            int numRecords = handleRecordNumberInput();
            RecordFactory::createRandomRecords(filename, numRecords);
        }
        else if (option == 2) {
            int numRecords = handleRecordNumberInput();
            RecordFactory::createManualRecords(filename, numRecords);
        }
        else if (option != 3) {
            std::cout << "Invalid option!\n";
        }
    }
}
