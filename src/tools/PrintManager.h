#pragma once
#include "../sorting/Tape.h"

class PrintManager
{
public:
	static void displayPhaseHeader(const int& phaseNumber);
	static bool askUserForDisplay();
	static void displayTapeState(Tape* tape1, Tape* tape2, Tape* outputTape);
	static void displaySortedTape(Tape* outputTape, const int& phaseNumber);
};

