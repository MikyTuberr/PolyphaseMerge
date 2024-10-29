#include "SortingManager.h"

void SortingManager::sortDataFromFile(std::ifstream& read_file)
{
    Tape tape1("./src/data/tape1.bin");
    Tape tape2("./src/data/tape2.bin");

    tape1.open({ std::ios::binary, std::ios::out });
    tape2.open({ std::ios::binary, std::ios::out });
    DistributionManager distributionManager(&tape1, &tape2);
    distributionManager.distributeSeriesWithFibonacci(read_file);
    tape1.close();
    tape2.close();
    std::cout << "\n\n=====================\n\n";
    printRecords("src/data/tape1.bin");
    std::cout << "\n\n=====================\n\n";
    printRecords("src/data/tape2.bin");
    std::cout << "\n\n=====================\n\n";
}
