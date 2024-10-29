#pragma once
#include "RecordFactory.h"

class RecordManager
{
private:
	static void displayOptions();
	static int handleOptionInput();
	static int handleRecordNumberInput();
public:
	static void handleRecordCreation(const std::string& filename);
};

