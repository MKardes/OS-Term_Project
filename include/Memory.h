#ifndef MEMORY_H
#define MEMORY_H

#include "AMemoryBlock.h"
#include "InstructionBlock.h"
#include "DataBlock.h"
#include <vector>
#include <map>

class Memory {
private:
    std::vector<AMemoryBlock*> os_blocks; // 0-999
    std::vector<std::vector<AMemoryBlock*>> memory_blocks; // 1000-10999

    // Each memory block for 1000 size has those structures:
    // * 0: start of the data section
    // * 1 - (os_blocks[0] - 1): instruction section
    // * os_blocks[0] - (stack_pointer - 1): data section
    // * stack_pointer - 1000: stack (stack pointer is the top of the stack)

    // OS Data Section:
    // * 0-20 registers:
    //   -- 0: program counter
    //   -- 1: stack pointer
    //   -- 2: system call result
    //   -- 3: Number of instructions executed so far
    //   -- 4: Thread number executing 0 for OS, 1 for thread 1, etc.
    // * 21 Other datas

    // Thread Memory:
public:
    Memory();
    ~Memory();
    void fillMemoryBlocks(int tn, std::vector<InstructionBlock> &instruction_blocks, std::vector<DataBlock> &data_blocks);
    void printMemoryBlocks(long unsigned int start = 0, long unsigned int end = 1000);
    void printMemoryBlocks(int tn, long unsigned int start = 0, long unsigned int end = 1000);
    DataBlock *getRegister(int address);
    void setRegister(int address, long value);
    DataBlock *getDataBlock(int tn, int address);
    void setDataBlock(int tn, int address, long value);
    InstructionBlock *getInstructionBlock(int tn, int n);
    InstructionBlock *getNextInstruction(int thread_number);
};

#endif
