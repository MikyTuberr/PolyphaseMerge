#include "FileIO.h"

FileIO::FileIO(FileIO&& other) noexcept :
    file(std::move(other.file)),
    filename(std::move(other.filename)),
    position(other.position),
    recordSize(other.recordSize),
    blockSize(other.blockSize),
    records(std::move(other.records))
{
    //other.position = 0;
}

FileIO& FileIO::operator=(FileIO && other) noexcept
{
    if (this != &other) {
        if (file.is_open()) {
            file.close();
        }
        file = std::move(other.file);
        filename = std::move(other.filename);
        position = other.position;
        recordSize = other.recordSize;
        blockSize = other.blockSize;
        records = std::move(other.records);
        //other.position = 0;
        file.close();
    }
    return *this;
}

FileIO::~FileIO()
{
    if (file.is_open()) {
        file.close();
    }
}

bool FileIO::isEmpty() const
{
    return _isEmpty;
}

void FileIO::printFile()
{
    bool stop = true;

    std::streampos tmpPos = position;
    bool tmpEof = isEof;

    resetPosition();

    while (stop) {
        std::vector<Record> records;
        stop = read(records, false);
        for (const auto& record : records) {
            record.print();
        }
    }

    isEof = tmpEof;
    position = tmpPos;
}

bool FileIO::open(const std::initializer_list<std::ios::openmode> modes)
{
    if (file.is_open()) {
        std::cout << "File already opened, closing.\n";
        file.close();
    }
    std::ios::openmode combinedMode = std::ios::binary;
    for (auto mode : modes) {
        combinedMode |= mode;
    }
    file.open(filename, combinedMode);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return false;
    }
    return true;
}

void FileIO::close()
{
    if (file.is_open()) {
        file.close();
    }
}

bool FileIO::read(std::vector<Record>& records, const bool& countPage)
{
    if (!file.is_open()) {
        throw std::runtime_error("File is not opened!\n.");
    }

    size_t bytesToRead = adjustBytesToRead(file);

    if (this->isEof) {
        //PrintManager::printEOFMessage();
        return false;
    }

    if (bytesToRead == 0) {
        //PrintManager::printInsufficientBytesMessage();
        return false;
    }

    std::vector<std::byte> buffer(bytesToRead);
    readBuffer(file, buffer, bytesToRead);

    size_t bytesRead = buffer.size();
    populateRecords(buffer, bytesRead, records);

    std::streamoff offset = (bytesRead / recordSize) * recordSize;
    position += offset;

    if (countPage) {
        pagesRead++;
    }
    return true;
}

void FileIO::writeRecord(const Record& record) 
{
    if (_isEmpty) {
		_isEmpty = false;
	}

    this->records.push_back(record);

    if (this->records.size() >= blockSize / recordSize) {
        this->flush();
    }
}

void FileIO::flush()
{
    this->writeBlock();
    records = std::vector<Record>();
}

void FileIO::writeBlock() {

    if (!file.is_open()) {
        throw std::runtime_error("File is not opened!\n");
    }

    try {
        for (const auto& record : records) {
            std::vector<double> data = record.getData();

            for (const auto& value : data) {
                file.write(reinterpret_cast<const char*>(&value), sizeof(value));

                if (!file) {
                    throw std::runtime_error("Error writing to file!\n");
                }
            }
        }
        pagesWritten++;
    }
    catch (const std::exception& e) {
        throw e;
    }
}

void FileIO::resetPosition()
{
    this->isEof = false;
    this->position = 0;
}

size_t FileIO::getPagesWritten() const
{
    return this->pagesWritten;
}

size_t FileIO::getPagesRead() const
{
    return this->pagesRead;
}

size_t FileIO::adjustBytesToRead(std::fstream& file)
{
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(position);
    size_t bytesToRead = blockSize;

    if (static_cast<std::streamoff>(position) + static_cast<std::streamoff>(bytesToRead) >
        static_cast<std::streamoff>(fileSize))
    {
        bytesToRead = static_cast<size_t>(fileSize - position);

        if (bytesToRead == 0) {
            this->isEof = true;
            return bytesToRead;
        }

        //PrintManager::printAdjustedBytesMessage(bytesToRead, blockSize);
    }

    size_t bytesToReadMultiple = (bytesToRead / recordSize) * recordSize;

    if (bytesToReadMultiple != bytesToRead) {
        //PrintManager::printAdjustedRecordSizeMessage(bytesToRead, bytesToReadMultiple);
        bytesToRead = bytesToReadMultiple;
    }

    return bytesToRead;
}

void FileIO::readBuffer(std::fstream& file, std::vector<std::byte>& buffer, size_t bytesToRead)
{
    file.read(reinterpret_cast<char*>(buffer.data()), bytesToRead);
    if (!file) {
        throw std::runtime_error("Error reading file!\n");
    }
}

void FileIO::populateRecords(const std::vector<std::byte>& buffer, size_t bytesRead, std::vector<Record>& records)
{
    size_t recordsToRead = bytesRead / recordSize;

    for (size_t i = 0; i < recordsToRead; ++i) {
        std::vector<double> values(3);
        std::memcpy(values.data(), buffer.data() + (i * recordSize), recordSize);
        records.push_back(Record(values));
    }
}
