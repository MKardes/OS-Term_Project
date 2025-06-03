#include "Memory.h"

Memory::Memory() {
    memory_blocks = std::vector<std::vector<AMemoryBlock*>>(10);
}

Memory::~Memory() {
    for (long unsigned int i = 0; i < memory_blocks.size(); i++) {
        for (long unsigned int j = 0; j < memory_blocks[i].size(); j++) {
            delete memory_blocks[i][j];
        }
    }
    for (long unsigned int i = 0; i < os_blocks.size(); i++) {
        delete os_blocks[i];
    }
}

void Memory::fillMemoryBlocks(int tn, std::vector<InstructionBlock> &instruction_blocks, std::vector<DataBlock> &data_blocks) {
    bool is_os = tn == 0 ? true : false;
    tn = tn - 1;

    if (is_os) {
        // It is OS's data
        os_blocks = std::vector<AMemoryBlock*>(1000, nullptr);

        int data_block_size = data_blocks.size();
        int data_block_start = instruction_blocks.size() + 1;
        os_blocks[0] = new DataBlock(0, data_block_start);

        for (int i = 1; i < data_block_start; i++) {
            os_blocks[i] = new InstructionBlock(instruction_blocks[i - 1]); // For instruction section
        }

        for (int i = 0; i < data_block_size; i++) {
            os_blocks[data_block_start + i] = new DataBlock(data_blocks[i]); // For data section
        }
        
        for (int i = data_block_size; data_block_start + i < 1000; i++) {
            os_blocks[data_block_start + i] = new DataBlock(i, 0);/* new DataBlock();  */ // For stack pointer 
            // TODO: it could be addressed or without address for stack examine again
        }
    } else {
        // It is a thread's data
        memory_blocks[tn] = std::vector<AMemoryBlock*>(1000, nullptr);

        int data_block_size = data_blocks.size();
        int data_block_start = instruction_blocks.size() + 1;
        memory_blocks[tn][0] = new DataBlock(0, data_block_start);

        for (int i = 1; i < data_block_start; i++) {
            memory_blocks[tn][i] = new InstructionBlock(instruction_blocks[i - 1]); // For instruction section
        }

        for (int i = 0; i < data_block_size; i++) {
            memory_blocks[tn][data_block_start + i] = new DataBlock(data_blocks[i]); // For data section
        }

        for (int i = data_block_size; data_block_start + i < 1000; i++) {
            memory_blocks[tn][data_block_start + i] = new DataBlock(i, 0);/*  new DataBlock();  */ // For stack pointer 
            // TODO: it could be addressed or without address for stack examine again
        }
    }

    instruction_blocks.clear();
    data_blocks.clear();
}

void Memory::printMemoryBlocks(long unsigned int start, long unsigned int end) {
    std::cout << "Memory blocks: " << start << " " << end << std::endl;
    for (long unsigned int i = 0; i < memory_blocks.size(); i++) {
        std::cout << "Thread " << i << ": " << std::endl;
        for (long unsigned int j = start; j < memory_blocks[i].size() && j < end; j++) {
            std::cout << memory_blocks[i][j]->toString() << std::endl;
        }
    }
}

void Memory::printMemoryBlocks(int tn, long unsigned int start, long unsigned int end) {
    bool is_os = tn == 0 ? true : false;
    tn = tn - 1;

    if (is_os) {
        std::cout << "Registers: ";
        for (long unsigned int i = 0; i < 21; i++) {
            std::cout << getRegister(i)->toShortString() << " ";
        }
        std::cout << std::endl;
        for (long unsigned int i = start; i < os_blocks.size() && i < end; i++) {
            std::cout << os_blocks[i]->toString() << std::endl;
        }
    } else {
        for (long unsigned int i = start; i < memory_blocks[tn].size() && i < end; i++) {
            std::cout << memory_blocks[tn][i]->toString() << std::endl;
        }
    }
}

void Memory::debug() {
    int mem = 0;
    std::cerr << "******************************************************************************" << std::endl;
    std::cerr << "Debug 1 Begin" << std::endl;
    std::cerr << "******************************************************************************" << std::endl;
    std::cerr << "---------------------------------" << std::endl;
    std::cerr << "OS Memory Begin" << std::endl;
    std::cerr << "---------------------------------" << std::endl;
    DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(os_blocks[0]);
    if (!data_block_pointer) {
        throw std::runtime_error("Data block pointer not found");
    }
    long start_of_data_section = data_block_pointer->getValue();
    long stack_pointer = getRegister(1)->getValue();


    std::cerr << mem++ << "." << os_blocks[0]->toString() << " # Start of data section" << std::endl;
    for (long unsigned int i = 1; i < start_of_data_section; i++) {
        std::cerr << mem++ << "." << os_blocks[i]->toString() << std::endl;
    }
    for (long unsigned int i = start_of_data_section; i < start_of_data_section + 21; i++) {
        std::cerr << mem++ << "." << os_blocks[i]->toString() << " # Register " << i - start_of_data_section << std::endl;
    }
    for (long unsigned int i = start_of_data_section + 21; i < stack_pointer + start_of_data_section; i++) {
        std::cerr << mem++ << "." << os_blocks[i]->toString() << std::endl;
    }
    for (long unsigned int i = stack_pointer + start_of_data_section; i < os_blocks.size(); i++) {
        std::cerr << mem++ << "." << os_blocks[i]->toString() << " # Stack" << std::endl;
    }

    std::cerr << "---------------------------------" << std::endl;
    std::cerr << "OS Memory End" << std::endl;
    std::cerr << "---------------------------------" << std::endl;

    for (int tn = 0; tn < 10; tn++) {
        std::cerr << "---------------------------------" << std::endl;
        std::cerr << "Thread " << tn << " Memory Begin" << std::endl;
        std::cerr << "---------------------------------" << std::endl;
        DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(memory_blocks[tn][0]);
        if (!data_block_pointer) {
            throw std::runtime_error("Data block pointer not found");
        }
        long start_of_data_section = data_block_pointer->getValue();
        long stack_pointer = getDataBlock(tn, 1)->getValue();


        std::cerr << mem++ << "." << memory_blocks[tn][0]->toString() << " # Start of data section" << std::endl;
        for (long unsigned int i = 1; i < start_of_data_section; i++) {
            std::cerr << mem++ << "." << memory_blocks[tn][i]->toString() << std::endl;
        }
        for (long unsigned int i = start_of_data_section; i < stack_pointer + start_of_data_section; i++) {
            std::cerr << mem++ << "." << memory_blocks[tn][i]->toString() << std::endl;
        }
        for (long unsigned int i = stack_pointer + start_of_data_section; i < memory_blocks[tn].size(); i++) {
            std::cerr << mem++ << "." << memory_blocks[tn][i]->toString() << " # Stack" << std::endl;
        }
        std::cerr << "---------------------------------" << std::endl;
        std::cerr << "Thread " << tn << " Memory End" << std::endl;
        std::cerr << "---------------------------------" << std::endl;
        std::cerr << "******************************************************************************" << std::endl;
        std::cerr << "Debug 1 End" << std::endl;
        std::cerr << "******************************************************************************" << std::endl;
        std::cerr << std::endl;
    }
}

DataBlock *Memory::getRegister(int address) {
    if (address < 0 || address > 20) throw std::runtime_error("Invalid register address: " + std::to_string(address));

    DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(os_blocks[0]);
    if (!data_block_pointer) {
        throw std::runtime_error("Data block pointer not found");
    }
    long start_of_data_section = data_block_pointer->getValue();

    DataBlock *data_block = dynamic_cast<DataBlock*>(os_blocks[start_of_data_section + address]);
    if (!data_block) {
        throw std::runtime_error("Register not found");
    }

    return data_block;
}

void Memory::setRegister(int address, long value) {
    if (address < 0 || address > 20) throw std::runtime_error("Invalid register address: " + std::to_string(address));

    DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(os_blocks[0]);
    if (!data_block_pointer) {
        throw std::runtime_error("Data block pointer not found");
    }
    long start_of_data_section = data_block_pointer->getValue();

    DataBlock *data_block = dynamic_cast<DataBlock*>(os_blocks[start_of_data_section + address]);
    if (!data_block) {
        throw std::runtime_error("Register not found");
    }

    data_block->setDataBlock(address, value);
}

DataBlock *Memory::getDataBlock(int tn, int address) {
    if (address < 0) throw std::runtime_error("Invalid address: " + std::to_string(address));

    bool is_os = tn == 0 ? true : false;
    tn = tn - 1;

    if (is_os) {
        // Get the data section to find program counter    
        DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(os_blocks[0]);
        if (!data_block_pointer) {
            throw std::runtime_error("Data block pointer not found");
        }
        long start_of_data_section = data_block_pointer->getValue();

        if (start_of_data_section + address /*+ 21*/ > 1000) {
            throw std::runtime_error("Address is out of bounds " + std::to_string(start_of_data_section + address /*+ 21*/));
        }

        DataBlock *data_block = dynamic_cast<DataBlock*>(os_blocks[start_of_data_section + address /*+ 21*/]); // + 21 for registers
        if (!data_block) {
            throw std::runtime_error("Data block not found");
        }
        return data_block;
    } else { 
        // Get the data section to find program counter    
        DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(memory_blocks[tn][0]);
        if (!data_block_pointer) {
            throw std::runtime_error("Data block pointer not found");
        }
        long start_of_data_section = data_block_pointer->getValue();

        if (start_of_data_section + address > 1000) {
            throw std::runtime_error("Address is out of bounds " + std::to_string(start_of_data_section + address));
        }

        DataBlock *data_block = dynamic_cast<DataBlock*>(memory_blocks[tn][start_of_data_section + address]);
        if (!data_block) {
            throw std::runtime_error("Data block not found");
        }
        return data_block;
    }
}

/** 
 * @brief Set the data block
 * @param tn The thread  0 if os, 1 if thread 1, etc.
 * @param address The address of the data block. 
 *  Will assume that the address is the index of the data block !!!!
 * @param value The value to set
 * @throws std::runtime_error
 **/ 
void Memory::setDataBlock(int tn, int address, long value) {
    if (address < 0) throw std::runtime_error("Invalid address: " + std::to_string(address));

    bool is_os = tn == 0 ? true : false;
    tn = tn - 1;

    if (is_os) {
        DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(os_blocks[0]);
        if (!data_block_pointer) {
            throw std::runtime_error("Data block pointer not found");
        }
        long start_of_data_section = data_block_pointer->getValue();

        if (start_of_data_section + address /*+ 21*/ > 1000) {
            throw std::runtime_error("Address is out of bounds " + std::to_string(start_of_data_section + address /*+ 21*/));
        }

        DataBlock *data_block = dynamic_cast<DataBlock*>(os_blocks[start_of_data_section + address /*+ 21*/]); // + 21 for registers
        if (!data_block) {
            throw std::runtime_error("Data block not found");
        }

        data_block->setDataBlock(address, value);
    } else {
        DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(memory_blocks[tn][0]);
        if (!data_block_pointer) {
            throw std::runtime_error("Data block pointer not found");
        }
        long start_of_data_section = data_block_pointer->getValue();

        if (start_of_data_section + address > 1000) {
            throw std::runtime_error("Address is out of bounds " + std::to_string(start_of_data_section + address));
        }

        DataBlock *data_block = dynamic_cast<DataBlock*>(memory_blocks[tn][start_of_data_section + address]);
        if (!data_block) {
            throw std::runtime_error("Data block not found");
        }

        data_block->setDataBlock(address, value);
    }
}

InstructionBlock *Memory::getInstructionBlock(int tn, int n) {
    bool is_os = tn == 0 ? true : false;
    tn = tn - 1;

    if (is_os) {
        InstructionBlock *instruction_block = dynamic_cast<InstructionBlock*>(os_blocks[1 + n]);
        if (!instruction_block) {
            throw std::runtime_error("Instruction block not found");
        }

        return instruction_block;
    } else {
        InstructionBlock *instruction_block = dynamic_cast<InstructionBlock*>(memory_blocks[tn][1 + n]);
        if (!instruction_block) {
            throw std::runtime_error("Instruction block not found");
        }

        return instruction_block;
    }
}

InstructionBlock *Memory::getNextInstruction(int tn) {
    DataBlock *pg_reg = getRegister(0); // Get first register from 0-20 of os_section
    if (!pg_reg) {
        throw std::runtime_error("Program counter register not found");
    }

    InstructionBlock *instruction_block = getInstructionBlock(tn, pg_reg->getValue()); 
    if (!instruction_block) {
        throw std::runtime_error("Instruction block not found");
    }

    return instruction_block;
}
