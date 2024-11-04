#include "sorting/SortingManager.h"
#include "tools/RecordManager.h"

int main() {
    std::string read_tape_filename = "src/data/data.bin";

    RecordManager::handleRecordCreation(read_tape_filename);

    std::string tape1_filename = "src/data/tape1.bin";
    std::string tape2_filename = "src/data/tape2.bin";
    std::string tape3_filename = "src/data/tape3.bin";

    try {
        SortingManager::sortDataFromFile(read_tape_filename, tape1_filename, 
            tape2_filename, tape3_filename);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what();
    }

    return 0;
}