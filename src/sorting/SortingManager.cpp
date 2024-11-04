#include "SortingManager.h"

void SortingManager::distribute(Tape* read_tape, Tape* tape1, Tape* tape2)
{
    read_tape->open({ std::ios::in });
    tape1->open({ std::ios::out });
    tape2->open({ std::ios::out });

    PrintManager::printStageHeader("STARTING DISTRIBUTION");

    DistributionManager distributionManager(read_tape, tape1, tape2);
    distributionManager.distributeSeriesWithFibonacci();

    PrintManager::printStageHeader("FINISHING DISTRIBUTION");

    read_tape->close();
    tape1->close();
    tape2->close();
}

void SortingManager::sort(Tape* tape1, Tape* tape2, Tape* outputTape)
{
    outputTape->open({ std::ios::out });
    outputTape->close();

    outputTape->open({ std::ios::out, std::ios::in });
    tape1->open({ std::ios::out, std::ios::in });
    tape2->open({ std::ios::out, std::ios::in });

    PrintManager::printStageHeader("STARTING SORTING");

    PolyphaseSorter polyphaseSorter(tape1, tape2, outputTape);
    polyphaseSorter.sortTapesWithFibonacci();

    PrintManager::printStageHeader("FINISHING SORTING");

    outputTape->close();
    tape1->close();
    tape2->close();
}

void SortingManager::sortDataFromFile(const std::string& read_filename, const std::string& tape1_filename,
    const std::string& tape2_filename, const std::string& tape3_filename)
{
    Tape read_tape(read_filename);
    Tape tape1(tape1_filename);
    Tape tape2(tape2_filename);
    Tape outputTape(tape3_filename);

    distribute(&read_tape, &tape1, &tape2);

    sort(&tape1, &tape2, &outputTape);

    std::filesystem::remove(tape1_filename);
    std::filesystem::remove(tape2_filename);
    std::filesystem::remove(tape3_filename);
}
