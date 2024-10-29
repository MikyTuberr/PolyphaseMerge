#include "SortingManager.h"

void SortingManager::sortDataFromFile(const std::string& read_filename, const std::string& tape1_filename, const std::string& tape2_filename)
{
    Tape read_tape(read_filename);
    Tape tape1(tape1_filename);
    Tape tape2(tape2_filename);

    read_tape.open({ std::ios::binary, std::ios::in });
    tape1.open({ std::ios::binary, std::ios::out });
    tape2.open({ std::ios::binary, std::ios::out });

    DistributionManager distributionManager(&read_tape, &tape1, &tape2);
    distributionManager.distributeSeriesWithFibonacci();

    read_tape.close();
    tape1.close();
    tape2.close();

    std::cout << "\n\n=====================\n\n";
    printRecords(tape1_filename);
    std::cout << "\n\n=====================\n\n";
    printRecords(tape2_filename);
    std::cout << "\n\n=====================\n\n";
}
