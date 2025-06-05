#ifndef MEMORY_H
#define MEMORY_H

#include "AMemoryBlock.h"
#include "InstructionBlock.h"
#include "DataBlock.h"
#include <vector>
#include <map>

class Memory {
private:
    const static int EXECUTED_INSTRUCTIONS = 20; // register 20
    const static int THREAD_NUMBER = 19; // Current running thread number
    const static int THREAD_TEMP_PC_SWITCH = 18; // Current running thread number
    const static int SYSCALL_TYPE = 17; // 0: hlt, 1: yield, 2: prn
    // const static int THREAD_NUMBER_SWITCH = 4; // Thread number that will be switched to
    const static int EXECUTED_INSTRUCTIONS_THREAD = 6; // register 6
    const static int STACK_POINTER = 1; // register 1
    const static int PROGRAM_COUNTER = 0; // register 0

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
    //   // TODO: it is not used-- 3: Number of instructions executed so far
    //   -- 4: Thread number This only used for context switch
    //   -- 5: Starting time(instruction) of that thread
    //   -- 6: Number of instructions executed so far (for that thread)
    //   -- 7: The state of the thread (ready, blocked, running) -> (rdy:0, blc:1, run:2)
    //   ...
    //   -- 19: The thread number executing 0 for OS, 1 for thread 1, etc.
    //   -- 20: Number of instructions executed so far
    // * 21 Other datas

    // Thread Memory:
public:
    Memory();
    ~Memory();
    void fillMemoryBlocks(int tn, std::vector<InstructionBlock> &instruction_blocks, std::vector<DataBlock> &data_blocks);
    void printMemoryBlocks(int tn, long unsigned int start = 0, long unsigned int end = 1000);
    void debug();
    DataBlock *getRegister(int address);
    void setRegister(int address, long value);
    DataBlock *getDataBlock(int tn, int address);
    void setDataBlock(int tn, int address, long value);
    InstructionBlock *getInstructionBlock(int tn, int n);
    InstructionBlock *getNextInstruction(int thread_number);
};

#endif
