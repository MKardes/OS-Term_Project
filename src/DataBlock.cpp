#include "DataBlock.h"
#include <sstream>

DataBlock::DataBlock(std::string instruction): AMemoryBlock(false) {
    std::stringstream ss(instruction);
    std::string address, value;
    ss >> address >> value;
    this->address = std::stol(address);
    this->value = std::stol(value);
    std::cout << "DataBlock created: " << this->address << ": " << this->value << std::endl;
}

DataBlock::DataBlock(long address, long value): AMemoryBlock(false) {
    this->address = address;
    this->value = value;
}

DataBlock::DataBlock(): AMemoryBlock(false) {
    this->address = 0;
    this->value = 0;
}

void DataBlock::setDataBlock(long address, long value) {
    this->address = address;
    this->value = value;
}

long DataBlock::getAddress() const {
    return address;
}

long DataBlock::getValue() const {
    return value;
}

std::string DataBlock::toString() const {
    std::stringstream ss;
    ss << "DataBlock[" << address << "] = " << value;
    return ss.str();
}

std::string DataBlock::toShortString() const {
    std::stringstream ss;
    ss << "[" << address << "] = " << value;
    return ss.str();
}

void DataBlock::setAddress(long addr) {
    this->address = addr;
}

void DataBlock::setValue(long value) {
    this->value = value;
}