#include "PrintManager.h"

void PrintManager::displayPhaseHeader(const int& phaseNumber) {
	std::cout << "\n\n////////////////////////////////PHASE " << phaseNumber << "////////////////////////////////\n\n";
}

bool PrintManager::askUserForDisplay() {
	char userInput;
	std::cout << "\nDo you want to display the tape states before moving to the next phase? (y/n): ";
	std::cin >> userInput;

	return (userInput == 'y' || userInput == 'Y');
}

void PrintManager::displayTapeState(Tape* tape1, Tape* tape2, Tape* outputTape) {
	std::cout << "\n\n================================ TAPE 1 =================================\n\n";
	tape1->print();
	std::cout << "\n\n================================ TAPE 2 =================================\n\n";
	tape2->print();
	std::cout << "\n\n================================ TAPE 3 (OUTPUT) =================================\n\n";
	outputTape->print();
}

void PrintManager::displaySortedTape(Tape* outputTape, const int& phaseNumber) {
	std::cout << "\n\n================================ SORTED TAPE =================================\n\n";
	outputTape->print();
}