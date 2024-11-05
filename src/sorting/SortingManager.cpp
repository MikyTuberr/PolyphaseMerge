#include "SortingManager.h"

void SortingManager::distribute(Tape* read_tape, Tape* tape1, Tape* tape2)
{
    read_tape->open({ std::ios::in });
    tape1->open({ std::ios::trunc, std::ios::out, std::ios::in });
    tape2->open({ std::ios::trunc, std::ios::out, std::ios::in });

    /*PrintManager::printTapeHeader("FILE");
    read_tape->print();
    PrintManager::printTapeHeader("FILE");*/

    PrintManager::printStageHeader("STARTING DISTRIBUTION");

    DistributionManager distributionManager(read_tape, tape1, tape2);
    distributionManager.distributeSeriesWithFibonacci();

    PrintManager::printStageHeader("FINISHING DISTRIBUTION");

    /*PrintManager::printTapeHeader("TAPE 1");
    tape1->print();
    PrintManager::printTapeHeader("TAPE 1");
    PrintManager::printTapeHeader("TAPE 2");
    tape2->print();
    PrintManager::printTapeHeader("TAPE 2");*/


    //PrintManager::printTapeContentMessage(tape1->getSeriesCounter(), tape1->getDummySeriesCounter());
    //PrintManager::printTapeContentMessage(tape2->getSeriesCounter(), tape2->getDummySeriesCounter());
    int sumOfSeries = tape1->getDummySeriesCounter() + tape1->getDummySeriesCounter() + 
        tape2->getSeriesCounter() + tape2->getDummySeriesCounter();

    std::cout << "SUM OF SERIES: " << sumOfSeries << "\n";
    read_tape->close();
    tape1->close();
    tape2->close();
}

int SortingManager::sort(Tape* tape1, Tape* tape2, Tape* outputTape)
{
    outputTape->open({ std::ios::trunc, std::ios::out, std::ios::in });
    tape1->open({ std::ios::out, std::ios::in });
    tape2->open({ std::ios::out, std::ios::in });

    PrintManager::printStageHeader("STARTING SORTING");

    PolyphaseSorter polyphaseSorter(tape1, tape2, outputTape);
    int phaseNumber = polyphaseSorter.sortTapesWithFibonacci();

    PrintManager::printStageHeader("FINISHING SORTING");

    outputTape->close();
    tape1->close();
    tape2->close();

    return phaseNumber;
}

void SortingManager::sortDataFromFile(const std::string& read_filename, const std::string& tape1_filename,
    const std::string& tape2_filename, const std::string& tape3_filename)
{
    Tape read_tape(read_filename);
    Tape tape1(tape1_filename);
    Tape tape2(tape2_filename);
    Tape outputTape(tape3_filename);

    distribute(&read_tape, &tape1, &tape2);

    size_t pagesWritten = tape1.getPagesWritten() + tape2.getPagesWritten();
    size_t pagesRead = read_tape.getPagesRead() + tape1.getPagesRead() + tape2.getPagesRead() + outputTape.getPagesRead();

    //std::cout << "PAGES WRITTEN: " << pagesWritten << "\n";
    //std::cout << "PAGES READ: " << pagesRead << "\n";

    int phaseNumber = sort(&tape1, &tape2, &outputTape);
    
    pagesWritten = tape1.getPagesWritten() + tape2.getPagesWritten() + outputTape.getPagesWritten();
    pagesRead = read_tape.getPagesRead() + tape1.getPagesRead() + tape2.getPagesRead() + outputTape.getPagesRead();

    std::cout << "NUMBER OF PHASES: " << phaseNumber << "\n";
    std::cout << "PAGES WRITTEN: " << pagesWritten << "\n";
    std::cout << "PAGES READ: " << pagesRead << "\n";

    //std::filesystem::remove(tape1_filename);
    //std::filesystem::remove(tape2_filename);
    //std::filesystem::remove(tape3_filename);
}
