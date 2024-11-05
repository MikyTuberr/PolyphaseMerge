#include "RecordFactory.h"

void RecordFactory::createRandomRecords(const std::string& filename, int recordsNumber)
{
    Tape tape(filename);
    tape.open({ std::ios::binary, std::ios::app });

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(DATA_DIST_BEG, DATA_DIST_END);

    for (int i = 0; i < recordsNumber; ++i) {
        double a = dist(gen);
        double b = dist(gen);
        double theta = dist(gen);

        tape.writeRecord(Record(a, b, theta));
    }
    tape.flush();
    tape.close();
    std::cout << recordsNumber << " random records written to file.\n\n"; \
}

void RecordFactory::createManualRecords(const std::string& filename, int recordsNumber)
{
    Tape tape(filename);
    tape.open({ std::ios::binary, std::ios::app });

    for (int i = 0; i < recordsNumber; ++i) {
        double a, b, theta;

        std::cout << "Record " << i + 1 << std::endl;
        std::cout << "Enter side length a: ";
        std::cin >> a;
        std::cout << "Enter side length b: ";
        std::cin >> b;
        std::cout << "Enter angle (in degrees): ";
        std::cin >> theta;

        tape.writeRecord(Record(a, b, theta));
    }
    tape.close();
    std::cout << recordsNumber << " manually entered records written to file.\n\n";
}
