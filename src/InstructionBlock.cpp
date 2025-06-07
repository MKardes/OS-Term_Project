#include "InstructionBlock.h"
#include <sstream>

std::string InstructionBlock::opCodeToString(OpCode op) {
    switch(op) {
        case SET: return "SET";
        case CPY: return "CPY";
        case CPYI: return "CPYI";
        case CPYI2: return "CPYI2";
        case ADD: return "ADD";
        case ADDI: return "ADDI";
        case SUBI: return "SUBI";
        case JIF: return "JIF";
        case PUSH: return "PUSH";
        case POP: return "POP";
        case CALL: return "CALL";
        case RET: return "RET";
        case HLT: return "HLT";
        case USER: return "USER";
        case SYSCALL: return "SYSCALL";
        case PRN: return "PRN";
        case YIELD: return "YIELD";
        default: return "UNKNOWN";
    }
}

OpCode InstructionBlock::stringToOpCode(const std::string& str) {
    if (str == "SET") return SET;
    if (str == "CPY") return CPY;
    if (str == "CPYI") return CPYI;
    if (str == "CPYI2") return CPYI2;
    if (str == "ADD") return ADD;
    if (str == "ADDI") return ADDI;
    if (str == "SUBI") return SUBI;
    if (str == "JIF") return JIF;
    if (str == "PUSH") return PUSH;
    if (str == "POP") return POP;
    if (str == "CALL") return CALL;
    if (str == "RET") return RET;
    if (str == "HLT") return HLT;
    if (str == "USER") return USER;
    if (str == "SYSCALL") return SYSCALL;
    if (str == "PRN") return PRN;
    if (str == "YIELD") return YIELD;
    throw std::invalid_argument("Unknown OpCode: " + str);
}

/**
 * @brief Construct a new InstructionBlock object
 * @param instruction The instruction to parse
 * @throws std::invalid_argument if the instruction opcode or operand is invalid
 * @throws std::out_of_range if the instruction operand is out of range
 */
InstructionBlock::InstructionBlock(std::string instruction): AMemoryBlock(true) {
    std::stringstream ss(instruction);
    std::string piece1, piece2, piece3, piece4, piece5;
    //piece1 is address
    ss >> piece1 >> piece2 >> piece3 >> piece4 >> piece5;
    this->address = std::stol(piece1);

    this->opcode1 = InstructionBlock::stringToOpCode(piece2);
    switch (this->opcode1) {
        case OpCode::SET:
        case OpCode::CPY:
        case OpCode::CPYI:
        case OpCode::CPYI2:
        case OpCode::ADD:
        case OpCode::ADDI:
        case OpCode::SUBI:
        case OpCode::JIF:
            this->operand1 = std::stol(piece3);
            this->operand2 = std::stol(piece4);
            // std::cout << "InstructionBlock created (2 operands): " << \
            InstructionBlock::opCodeToString(this->opcode1) << " " << \
            this->operand1 << " " << \
            this->operand2 << std::endl;
            break;
        case OpCode::PUSH:
        case OpCode::POP:
        case OpCode::CALL:
        case OpCode::USER:
            this->operand1 = std::stol(piece3);
            // std::cout << "InstructionBlock created (1 operand): " << \
            InstructionBlock::opCodeToString(this->opcode1) << " " << \
            this->operand1 << std::endl;
            break;
        case OpCode::SYSCALL:
            this->opcode2 = InstructionBlock::stringToOpCode(piece3);
            if (this->opcode2 == OpCode::PRN) {
                this->operand1 = std::stol(piece4);
            }
            // std::cout << "InstructionBlock created (2 opcodes): " << \
            InstructionBlock::opCodeToString(this->opcode1) << " " << \
            InstructionBlock::opCodeToString(this->opcode2);
            if (this->opcode2 == OpCode::PRN) {
                // std::cout << " " << this->operand1;
            }
            // std::cout << std::endl;
            break;
        default:
            break;
    };
}

std::string InstructionBlock::toString() const {
    std::stringstream ss;
    ss << "InstructionBlock[" << address << "] = " << opCodeToString(opcode1);
    
    switch (opcode1) {
        case SET:
        case CPY:
        case CPYI:
        case CPYI2:
        case ADD:
        case ADDI:
        case SUBI:
        case JIF:
            ss << " " << operand1 << " " << operand2;
            break;
        case PUSH:
        case POP:
        case CALL:
        case USER:
            ss << " " << operand1;
            break;
        case SYSCALL:
            ss << " " << opCodeToString(opcode2);
            break;
        default:
            break;
    }
    
    return ss.str();
}

long InstructionBlock::getAddress() {
    return address;
}

OpCode InstructionBlock::getOpcode1() {
    return opcode1;
}

OpCode InstructionBlock::getOpcode2() {
    return opcode2;
}

long InstructionBlock::getOperand1() {
    return operand1;
}

long InstructionBlock::getOperand2() {
    return operand2;
}