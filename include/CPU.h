#ifndef CPU_H
#define CPU_H

#include <string>
#include <vector>
#include "Memory.h"

class CPU {
private:
    const static int EXECUTED_INSTRUCTIONS = 20; // register 20
    const static int THREAD_NUMBER = 19; // Current running thread number
    const static int THREAD_TEMP_PC_SWITCH = 18; // Current running thread number
    const static int SYSCALL_TYPE = 17; // 0: hlt, 1: , 2: prn
    const static int THREAD_NUMBER_SWITCH = 4; // Thread number that will be switched to
    const static int EXECUTED_INSTRUCTIONS_THREAD = 6; // register 6
    const static int STACK_POINTER = 1; // register 1
    const static int PROGRAM_COUNTER = 0; // register 0

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