#include "storage/Tape.h"

int main() {
    Tape tape("file.bin");

    // Zapis
    if (tape.open({ std::ios::binary, std::ios::out })) {
        std::cout << "WRITE SUCCESS!\n";

        Parallelogram parallelogram(3.0, 4.0, 60.0);
        Record record(parallelogram);

        std::vector<Record> data = { record };
        if (tape.write(data)) {
            std::cout << "Record written successfully.\n";
        }
        else {
            std::cout << "Failed to write record.\n";
        }

        tape.close();
    }
    else {
        std::cout << "WRITE FAIL!\n";
    }

    // Odczyt
    if (tape.open({ std::ios::binary, std::ios::in })) {
        std::cout << "READ SUCCESS!\n";

        std::vector<Record> readData;
        if (tape.read(readData, 1)) {
            std::cout << "Read record: "; 
            readData[0].print();
        }
        else {
            std::cout << "Failed to read record.\n";
        }

        tape.close();
    }
    else {
        std::cout << "READ FAIL!\n";
    }

	return 0;
}