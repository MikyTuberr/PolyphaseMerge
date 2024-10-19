#include "RecordFactory.h"

void RecordFactory::createRandomRecords(const std::string& filename, int count)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(1.0, 100.0);

    for (int i = 0; i < count; ++i) {
        double a = dist(gen);
        double b = dist(gen);
        double theta = dist(gen);

        Record record(a, b, theta);
        FileIO::write(filename, { record });
    }

    std::cout << count << " random records written to file.\n";
}

void RecordFactory::createManualRecords(const std::string& filename, int count)
{
    for (int i = 0; i < count; ++i) {
        double a, b, theta;

        std::cout << "Record " << i + 1 << std::endl;
        std::cout << "Enter side length a: ";
        std::cin >> a;
        std::cout << "Enter side length b: ";
        std::cin >> b;
        std::cout << "Enter angle (in degrees): ";
        std::cin >> theta;

        Record record(a, b, theta);
        FileIO::write(filename, { record });
    }

    std::cout << count << " manually entered records written to file.\n";
}
