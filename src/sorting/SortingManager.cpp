#include "SortingManager.h"

void SortingManager::sortDataFromFile(const std::string& read_filename, const std::string& tape1_filename, const std::string& tape2_filename)
{
    Tape read_tape(read_filename);
    Tape tape1(tape1_filename);
    Tape tape2(tape2_filename);
    Tape outputTape("src/data/tape3.bin");

    read_tape.open({ std::ios::binary, std::ios::in });
    outputTape.open({ std::ios::binary, std::ios::out });
    tape1.open({ std::ios::binary, std::ios::out });
    tape2.open({ std::ios::binary, std::ios::out });

    std::cout << "\n*************** STARTING DISTRIBUTION ***************\n\n";

    DistributionManager distributionManager(&read_tape, &tape1, &tape2);
    distributionManager.distributeSeriesWithFibonacci();

    std::cout << "\n*************** FINISHING DISTRIBUTION ***************\n";

    read_tape.close();
    outputTape.close();
    tape1.close();
    tape2.close();

    outputTape.open({ std::ios::binary, std::ios::out, std::ios::in });
    tape1.open({ std::ios::binary, std::ios::out, std::ios::in });
    tape2.open({ std::ios::binary, std::ios::out, std::ios::in });

    std::cout << "\n*************** STARTING SORTING ***************\n\n";

    PolyphaseSorter polyphaseSorter(&tape1, &tape2, &outputTape);
    polyphaseSorter.sortTapesWithFibonacci();

    std::cout << "\n*************** FINISHING SORTING ***************\n";

    outputTape.close();
    tape1.close();
    tape2.close();

    std::filesystem::remove(tape1_filename);
    std::filesystem::remove(tape2_filename);
    std::filesystem::remove("src/data/tape3.bin");
}
