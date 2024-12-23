#include "PrintManager.h"

void PrintManager::printStageHeader(const std::string& title)
{
	std::cout << "\n*************** " << title << " ***************\n\n";
}

void PrintManager::printPhaseHeader(const int& phaseNumber) {
	std::cout << "\n\n################################ PHASE " << phaseNumber << " ################################\n\n";
}

bool PrintManager::promptUserDisplayDecision() {
	char userInput;
	std::cout << "\nDo you want to display the tape states before moving to the next phase? (y/n): ";
	std::cin >> userInput;

	return (userInput == 'y' || userInput == 'Y');
}

void PrintManager::printTapeHeader(const std::string& header) {
	std::cout << "\n\n================================ " << header << " ================================ = \n\n";
}

void PrintManager::printTapeContentMessage(const std::string& tapeName, const int& seriesNumber, const int& dummySeriesNumber, const int& recordsNumber)
{
	std::cout << "\n\n" << tapeName << ":\nSERIES : " << seriesNumber << " (" << dummySeriesNumber << ") RECORDS : " << recordsNumber << "\n";
}

void PrintManager::printAdjustedBytesMessage(const int& bytesToRead, const int& blockSize)
{
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	std::cout << "~~~ Adjusting read bytes to " << bytesToRead << ", due to insufficient block size (" << blockSize << "). ~~~\n";
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

void PrintManager::printAdjustedRecordSizeMessage(const int& bytesToRead, const int& bytesToReadMultiple)
{
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	std::cout << "~~~ Bytes to read (" << bytesToRead << ") not a multiple of record size. Adjusted to " << bytesToReadMultiple << ". ~~~\n";
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

void PrintManager::printInsufficientBytesMessage()
{
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	std::cout << "~~~ Insufficient bytes to read a full record.\n. ~~~\n";
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

void PrintManager::printEOFMessage()
{
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	std::cout << "~~~ End of file detected, stopping read operation. ~~~\n";
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}
