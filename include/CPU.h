#ifndef CPU_H
#define CPU_H

#include <string>
#include <vector>
#include "Memory.h"

class CPU {
private:
    Memory memory;
    bool halted;
    bool kernel_mode;
    int debug_level;

    void execute_instruction(long tn, InstructionBlock *instruction);
    int getProgramCounter();

public:
    CPU(std::string filename, int debug_level);
    void execute();
    bool isHalted();
    void syscall();
    void debug();
};

#endif