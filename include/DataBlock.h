#ifndef DATA_BLOCK_H
#define DATA_BLOCK_H

#include "AMemoryBlock.h"

class DataBlock: public AMemoryBlock {
private:
    long address;
    long value;

public:
    DataBlock(std::string instruction);
    DataBlock(long address, long value);
    DataBlock();
    std::string toString() const override;
    std::string toShortString() const;
    void setDataBlock(long address, long value);
    long getValue() const;
};

#endif