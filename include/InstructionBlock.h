#ifndef INSTRUCTION_BLOCK_H
#define INSTRUCTION_BLOCK_H

#include "AMemoryBlock.h"

enum OpCode {
    SET,
    CPY,
    CPYI,
    CPYI2, // Optional
    ADD,
    ADDI,
    SUBI,
    JIF,
    PUSH,
    POP,
    CALL,
    RET,
    HLT,
    USER,
    SYSCALL,
    PRN,
    YIELD
};

class InstructionBlock: public AMemoryBlock {
private:
    OpCode opcode1;
    OpCode opcode2;
    long operand1;
    long operand2;

public:
    InstructionBlock(std::string instruction);
    static OpCode stringToOpCode(const std::string& str);
    static std::string opCodeToString(OpCode op);
    std::string toString() const override;
    OpCode getOpcode1();
    OpCode getOpcode2();
    long getOperand1();
    long getOperand2();
};

#endif