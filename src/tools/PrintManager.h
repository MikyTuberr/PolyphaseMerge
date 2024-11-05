#pragma once
#include <iostream>

class PrintManager
{
public:
	static void printStageHeader(const std::string& title);

	static void printPhaseHeader(const int& phaseNumber, const int& tape1Series, const int& tape1DummySeries, 
		const int& tape2Series, const int& tape2DummySeries, const int& tape3Series, const int& tape3DummySeries);
	static bool promptUserDisplayPhasesDecision();
	static bool promptUserDisplayTapesDecision();

	static void printTapeHeader(const std::string& header);
	static void printTapeContentMessage(const int& seriesNumber, const int& dummySeriesNumber);

	static void printAdjustedBytesMessage(const int& bytesToRead, const int& blockSize);
	static void printAdjustedRecordSizeMessage(const int& bytesToRead, const int& bytesToReadMultiple);
	static void printInsufficientBytesMessage();
	static void printEOFMessage();
};

