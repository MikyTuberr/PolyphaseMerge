#include "PrintManager.h"

void PrintManager::printStageHeader(const std::string& title)
{
	std::cout << "\n****************************** " << title << " ******************************\n\n";
}

void PrintManager::printPhaseHeader(const int& phaseNumber, const int& tape1Series, const int& tape1DummySeries,
	const int& tape2Series, const int& tape2DummySeries, const int& tape3Series, const int& tape3DummySeries) {
	std::cout << "\n################################ PHASE " << phaseNumber << " [1: " << tape1Series << " (" <<
		tape1DummySeries << ") " << "2: " << tape2Series << " (" <<
		tape2DummySeries << ") " << "3: " << tape3Series << " (" <<
		tape3DummySeries << ")]" << " ################################\n\n";
}

bool PrintManager::promptUserDisplayPhasesDecision() {
	char userInput;
	std::cout << "Do you want to display the phases? (y/n): ";
	std::cin >> userInput;

	return (userInput == 'y' || userInput == 'Y');
}

bool PrintManager::promptUserDisplayTapesDecision() {
	char userInput;
	std::cout << "Do you want to display the tape states before moving to the next phase? (y/n): ";
	std::cin >> userInput;

	return (userInput == 'y' || userInput == 'Y');
}

void PrintManager::printTapeHeader(const std::string& header) {
	std::cout << "\n================================ " << header << " ================================= \n\n";
}

void PrintManager::printTapeContentMessage(const int& seriesNumber, const int& dummySeriesNumber)
{
	std::cout << "SERIES : " << seriesNumber << " (" << dummySeriesNumber << ")\n";
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
