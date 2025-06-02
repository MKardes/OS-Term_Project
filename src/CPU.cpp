#include "CPU.h"
#include "InstructionBlock.h"
#include "DataBlock.h"
#include <fstream>

/**
 * @brief Construct a new CPU object
 * @param filename The name of the file to load
 * @throws std::invalid_argument if an instruction is invalid in the file
 * @throws std::out_of_range if an instruction operand is out of range
 * @throws std::runtime_error if the file is not found
 */
CPU::CPU(std::string filename): halted(false), kernel_mode(true) {
    std::vector<DataBlock> data_blocks;
    std::vector<InstructionBlock> instruction_blocks;

    std::ifstream file(filename);
    std::string line;

    bool isInstructionSection = false;
    bool isDataSection = false;
    bool instructionSectionEnded = false;
    bool dataSectionEnded = false;

    int line_number = 0;
    int thread_number = -1;

    while (std::getline(file, line)) {
        line_number++;
        if (line == "Begin Instruction Section") {
            if (isInstructionSection || isDataSection) {
                throw std::runtime_error("Invalid file format on line " + std::to_string(line_number) + ": " + line);
            }

            isInstructionSection = true;
        } else if (line == "End Instruction Section") {
            if (!isInstructionSection || isDataSection) {
                throw std::runtime_error("Invalid file format on line " + std::to_string(line_number) + ": " + line);
            }

            instructionSectionEnded = true;
            isInstructionSection = false;
            thread_number++;

            // int instruction_blocks_size = instruction_blocks.size();
            // int data_blocks_size = data_blocks.size();
            memory.fillMemoryBlocks(thread_number, instruction_blocks, data_blocks);
            // memory.printMemoryBlocks(thread_number, 0, instruction_blocks_size + data_blocks_size + 1);
        } else if (line == "Begin Data Section") {
            if (isInstructionSection || isDataSection) {
                throw std::runtime_error("Invalid file format on line " + std::to_string(line_number) + ": " + line);
            }

            dataSectionEnded = false;
            instructionSectionEnded = false;
            isDataSection = true;

        } else if (line == "End Data Section") {
            if (isInstructionSection || !isDataSection) {
                throw std::runtime_error("Invalid file format on line " + std::to_string(line_number) + ": " + line);
            }

            dataSectionEnded = true;
            isDataSection = false;
        } else {
            if (!isInstructionSection && !isDataSection) {
                throw std::runtime_error("Invalid file format on line " + std::to_string(line_number) + ": " + line);
            }
            if (isInstructionSection) {
                instruction_blocks.push_back(InstructionBlock(line));
            } else if (isDataSection) {
                data_blocks.push_back(DataBlock(line));
            }
        }
    }

    if (isInstructionSection || isDataSection || !instructionSectionEnded || !dataSectionEnded) {
        std::cout << "isInstructionSection: " << isInstructionSection << std::endl;
        std::cout << "isDataSection: " << isDataSection << std::endl;
        std::cout << "instructionSectionEnded: " << instructionSectionEnded << std::endl;
        std::cout << "dataSectionEnded: " << dataSectionEnded << std::endl;
        throw std::runtime_error("Invalid file format. Incomplete file.");
    }

    if (thread_number == -1) {
        throw std::runtime_error("OS not found");
    } else if (thread_number > 10) {
        throw std::runtime_error("Too many threads! (max 10)");
    } else if (thread_number < 10) {
        std::cout << "Warning: Thread count is not enough!" << std::endl;
        std::cout << 10 - thread_number << " threads will be created with SYSCALL HLT" << std::endl;

        for (int i = thread_number + 1; i <= 10; i++) {
            instruction_blocks.push_back(InstructionBlock("SYSCALL HLT"));
            data_blocks.push_back(DataBlock("0 0"));

            memory.fillMemoryBlocks(i, instruction_blocks, data_blocks);
            // memory.printMemoryBlocks(i, 0, 3);
        }
    }
}

int CPU::getProgramCounter() {
    return memory.getRegister(0)->getValue();
}

void CPU::execute_instruction(long tn, InstructionBlock *instruction) {
    std::cout << "Executing instruction: " << instruction->toString() << " on thread " << tn << std::endl;
    int computed_pg = getProgramCounter() + 1;
    long value = 0;
    long value_2 = 0;
    DataBlock *data_block = nullptr;
    long inner_address_1 = 0;
    long inner_address_2 = 0;

    switch (instruction->getOpcode1()) {
        case OpCode::SET:
            // SET B A: Direct Set - Set memory location with a number
            memory.setDataBlock(tn, instruction->getOperand2(), instruction->getOperand1());
            break;
        case OpCode::CPY:
            // CPY A1 A2: Direct Copy - Copy content from one memory location to another
            data_block = memory.getDataBlock(tn, instruction->getOperand1());
            value = data_block->getValue();
            memory.setDataBlock(tn, instruction->getOperand2(), value);
            break;
        case OpCode::CPYI:
            // CPYI A1 A2: Indirect Copy - Copy content using memory address as index
            data_block = memory.getDataBlock(tn, instruction->getOperand1());
            inner_address_1 = data_block->getValue();
            data_block = memory.getDataBlock(tn, inner_address_1);
            value = data_block->getValue();
            memory.setDataBlock(tn, instruction->getOperand2(), value);
            break;
        case OpCode::CPYI2:
            // CPYI2 A1 A2: Indirect Copy 2 - Copy using both addresses as indices
            data_block = memory.getDataBlock(tn, instruction->getOperand1());
            inner_address_1 = data_block->getValue();
            data_block = memory.getDataBlock(tn, inner_address_1);
            value = data_block->getValue();
            data_block = memory.getDataBlock(tn, instruction->getOperand2());
            inner_address_2 = data_block->getValue();
            memory.setDataBlock(tn, inner_address_2, value);
            break;
        case OpCode::ADD:
            // ADD A B: Add number to memory location
            data_block = memory.getDataBlock(tn, instruction->getOperand1());
            value = data_block->getValue();
            memory.setDataBlock(tn, instruction->getOperand1(), value + instruction->getOperand2());
            break;
        case OpCode::ADDI:
            // ADDI A1 A2: Indirect Add - Add contents of memory addresses
            data_block = memory.getDataBlock(tn, instruction->getOperand1());
            value = data_block->getValue();
            data_block = memory.getDataBlock(tn, instruction->getOperand2());
            value_2 = data_block->getValue();
            memory.setDataBlock(tn, instruction->getOperand1(), value + value_2);
            break;
        case OpCode::SUBI:
            // SUBI A1 A2: Indirect Subtraction - Subtract contents of memory addresses
            data_block = memory.getDataBlock(tn, instruction->getOperand1());
            value = data_block->getValue();
            data_block = memory.getDataBlock(tn, instruction->getOperand2());
            value_2 = data_block->getValue();
            memory.setDataBlock(tn, instruction->getOperand2(), value - value_2);
            break;
        case OpCode::JIF:
            // JIF A C: Jump if memory location content is less than or equal to 0
            if (memory.getDataBlock(tn, instruction->getOperand1())->getValue() <= 0) {
                std::cout << "Jumping to " << instruction->getOperand2() << std::endl;
                computed_pg = instruction->getOperand2();
            }
            break;
        case OpCode::PUSH:
            // PUSH A: Push memory value onto stack
            // TODO: Implement PUSH
            break;
        case OpCode::POP:
            // POP A: Pop value from stack into memory
            // TODO: Implement POP
            break;
        case OpCode::CALL:
            // CALL C: Call subroutine at specified instruction
            // TODO: Implement CALL
            break;
        case OpCode::RET:
            // RET: Return from subroutine
            // TODO: Implement RET
            break;
        case OpCode::HLT:
            // HLT: Halts the CPU execution
            halted = true;
            break;
        case OpCode::USER:
            // USER A: Switch to user mode and jump to specified address
            // TODO: Implement USER
            break;
        case OpCode::SYSCALL:
            switch (instruction->getOperand2()) {
                case OpCode::PRN:
                    // SYSCALL PRN A: System call to print memory contents
                    // TODO: Implement PRN
                    break;
                case OpCode::HLT:
                    // SYSCALL HLT: Calls the operting system service. Shuts down the thread 
                    halted = true;
                    break;
                case OpCode::YIELD:
                    // SYSCALL YIELD: Yields CPU control to OS for thread scheduling
                    // TODO: Implement yield
                    break;
                default:
                    throw std::runtime_error("Invalid system call");
            }
            break;
        default:
            throw std::runtime_error("Invalid instruction");
    }
    
    memory.setRegister(0, computed_pg); // Set the program counter on register 0
}

void CPU::execute() {
    InstructionBlock *instruction = memory.getNextInstruction(0);
    
    long thread = memory.getRegister(4)->getValue(); // Get the thread number
    execute_instruction(thread, instruction);
    std::cout << "After execution: " << std::endl;
    memory.printMemoryBlocks(thread, 34, 55);
    std::cout << "After execution: " << std::endl;
}

bool CPU::isHalted() {
    return halted;
}