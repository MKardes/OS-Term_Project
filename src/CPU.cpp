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
CPU::CPU(std::string filename, int debug_level): halted(false), kernel_mode(true), debug_level(debug_level) {
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

        // Skip lines that start with # (comments)
        size_t firstNonSpace = line.find_first_not_of(" \t");
        if (firstNonSpace == std::string::npos || line[firstNonSpace] == '#') {
            continue;
        }

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
            instruction_blocks.push_back(InstructionBlock("0 SYSCALL HLT"));
            data_blocks.push_back(DataBlock("0 0"));
            data_blocks.push_back(DataBlock("0 997"));

            memory.fillMemoryBlocks(i, instruction_blocks, data_blocks);
            // memory.printMemoryBlocks(i, 0, 3);
        }
    }
}

int CPU::getProgramCounter() {
    return memory.getRegister(CPU::PROGRAM_COUNTER)->getValue();
}

void CPU::execute_instruction(long tn, InstructionBlock *instruction) {
    std::cout << "Executing instruction: " << instruction->toString() << " on thread " << tn << std::endl;
    int computed_pg = getProgramCounter() + 1;
    long value = 0;
    long value_2 = 0;
    DataBlock *data_block = nullptr;
    long inner_address_1 = 0;
    long inner_address_2 = 0;
    long stack_pointer = 0;
    long old_pc = 0;
    long user_tn = 0;
    // long user_pc = 0;

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
            stack_pointer = memory.getRegister(CPU::STACK_POINTER)->getValue();

            data_block = memory.getDataBlock(tn, instruction->getOperand1());
            value = data_block->getValue();
            memory.setDataBlock(tn, stack_pointer, value);

            memory.setRegister(1, stack_pointer - 1);
            break;
        case OpCode::POP:
            // POP A: Pop value from stack into memory
            stack_pointer = memory.getRegister(CPU::STACK_POINTER)->getValue();
            stack_pointer = stack_pointer + 1;

            data_block = memory.getDataBlock(tn, stack_pointer);
            value = data_block->getValue();
            memory.setDataBlock(tn, instruction->getOperand1(), value);

            memory.setRegister(1, stack_pointer);
            break;
        case OpCode::CALL:
            // CALL C: Call subroutine at specified instruction
            stack_pointer = memory.getRegister(CPU::STACK_POINTER)->getValue();
            memory.setDataBlock(tn, stack_pointer, computed_pg); // PUSH RETURN ADDRESS TO STACK
            memory.setRegister(1, stack_pointer - 1);

            computed_pg = instruction->getOperand1();
            break;
        case OpCode::RET:
            // RET: Return from subroutine
            stack_pointer = memory.getRegister(CPU::STACK_POINTER)->getValue();
            stack_pointer = stack_pointer + 1;
            data_block = memory.getDataBlock(tn, stack_pointer);
            value = data_block->getValue(); // POP RETURN ADDRESS FROM STACK
            memory.setRegister(1, stack_pointer);

            computed_pg = value;
            break;
        case OpCode::HLT:
            // HLT: Halts the CPU execution
            halted = true;
            break;
        case OpCode::USER:
            // USER A: Switch to user mode and jump to specified address
            data_block = memory.getDataBlock(tn, instruction->getOperand1());
            computed_pg = data_block->getValue();

            user_tn = memory.getRegister(CPU::THREAD_NUMBER_SWITCH)->getValue();

            memory.setRegister(CPU::THREAD_NUMBER, user_tn);    // CHANGE MODE TO that thread
            // memory.setRegister(CPU::PROGRAM_COUNTER, user_pc);  // CHANGE PC for that thread

            kernel_mode = false;
            break;
        case OpCode::SYSCALL:
            kernel_mode = true;
            old_pc = memory.getRegister(CPU::PROGRAM_COUNTER)->getValue();
            memory.setRegister(CPU::THREAD_TEMP_PC_SWITCH, old_pc);

            switch (instruction->getOpcode2()) {
                case OpCode::HLT:
                    // SYSCALL HLT: Calls the operting system service. Shuts down the thread 
                    memory.setRegister(CPU::SYSCALL_TYPE, 0);
                    break;
                case OpCode::YIELD:
                    // SYSCALL YIELD: Yields CPU control to OS for thread scheduling
                    memory.setRegister(CPU::SYSCALL_TYPE, 1);
                    break;
                case OpCode::PRN:
                    // SYSCALL PRN A: System call to print memory contents
                    memory.setRegister(CPU::SYSCALL_TYPE, 2);
                    data_block = memory.getDataBlock(tn, instruction->getOperand1());
                    value = data_block->getValue();
                    std::cout << "--------------Value--------------" << std::endl;
                    std::cout << "               " << value << std::endl;
                    std::cout << "---------------------------------" << std::endl;
                    // TODO: Implement: make thread sleep for 100 instructions
                    break;
                default:
                    throw std::runtime_error("Invalid system call");
            }

            computed_pg = 0;
            memory.setRegister(CPU::THREAD_NUMBER, 0); // Switch to OS thread
            break;
        default:
            throw std::runtime_error("Invalid instruction");
    }
    
    memory.setRegister(0, computed_pg); // Set the program counter on register 0
}

void CPU::execute() {
    long thread = memory.getRegister(CPU::THREAD_NUMBER)->getValue(); // Get the thread number,

    InstructionBlock *instruction = memory.getNextInstruction(thread); 
    execute_instruction(thread, instruction);

    // std::cout << "After execution: " << std::endl;
    // memory.printMemoryBlocks(thread, 125, 185);
    // memory.printMemoryBlocks(thread, 593, 601);
    // // memory.printMemoryBlocks(thread, 278, 280);
    // // memory.printMemoryBlocks(thread, 378, 380);
    // // std::cout << "Stack pointer: " << memory.getRegister(CPU::STACK_POINTER)->getValue() << std::endl;
    // // memory.printMemoryBlocks(thread, 995, 1000);
    // std::cout << "After execution: " << std::endl;

    long executed_instructions = memory.getRegister(CPU::EXECUTED_INSTRUCTIONS)->getValue();
    memory.setRegister(CPU::EXECUTED_INSTRUCTIONS, executed_instructions + 1);
    long executed_instructions_thread = memory.getRegister(CPU::EXECUTED_INSTRUCTIONS_THREAD)->getValue();
    memory.setRegister(CPU::EXECUTED_INSTRUCTIONS_THREAD, executed_instructions_thread + 1);
}

bool CPU::isHalted() {
    return halted;
}

void CPU::debug() {
    memory.debug();
}

// DataBlock[200] = 0
// DataBlock[201] = 101    tail = 100, tail.next 101, [tail.next] 102


// CPYI2 201 200 # [[200]] = [[201]] = 104
// SET 20 202
// CPYI 201 203  # [203] = [[201]] = 104
// CPYI2 202 201 # [[201]] = [[202]] = 0
// JIF 203 X

// #change tail to [200] - 1 (101 - 1)
// X CPY 200 204 # [204] = [200] = 101
// ADD 204 -1    # [204] = 100
// CPY 204 99
// DataBlock[200] = 101 
// DataBlock[201] = 103 [101] = [103] && [103] = 0

// DataBlock[200] = 103 
// DataBlock[201] = 105 [103] = [105] && [105] = 0

// .
// .
// .

// DataBlock[200] = 119
// DataBlock[201] = 121 [119] = [121] && [121] = 0 ==>> jif 121 X (if it is tail)