#pragma once
#include <iostream>

class PrintManager
{
public:
	static void printStageHeader(const std::string& title);

	static void printPhaseHeader(const int& phaseNumber);
	static bool promptUserDisplayDecision();

	static void printTapeHeader(const std::string& header);
	static void printTapeContentMessage(const std::string& tapeName, const int& seriesNumber, const int& dummySeriesNumber, const int& recordsNumber);

	static void printAdjustedBytesMessage(const int& bytesToRead, const int& blockSize);
	static void printAdjustedRecordSizeMessage(const int& bytesToRead, const int& bytesToReadMultiple);
	static void printInsufficientBytesMessage();
	static void printEOFMessage();
};

