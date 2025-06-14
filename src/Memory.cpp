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
        int max_inst_addr = -1;

        for (int i = 1; i <= instruction_blocks.size(); i++) {
            int addr = instruction_blocks[i - 1].getAddress();

            if (addr > 500) throw std::runtime_error("Instruction address is out of bounds: " + std::to_string(addr));
            if (addr > max_inst_addr) max_inst_addr = addr;

            os_blocks[addr + 1] = new InstructionBlock(instruction_blocks[i - 1]); // For instruction section
        }

        int data_block_start = max_inst_addr + 2; // +1 for instruction section, +1 for data_block_start
        os_blocks[0] = new DataBlock(0, data_block_start);

        for (int i = 0; i < data_blocks.size(); i++) {
            int addr = data_blocks[i].getAddress();

            if (data_block_start + addr > 999) throw std::runtime_error("DataBlock address is out of bounds: " + std::to_string(addr));

            os_blocks[data_block_start + addr] = new DataBlock(data_blocks[i]); // For data section
        }
        
        for (int i = data_block_start; i < 1000; i++) {
            if (os_blocks[i] == nullptr) {
                os_blocks[i] = new DataBlock(i - data_block_start, 0);/* new DataBlock();  */ // For stack pointer 
            }
            // TODO: it could be addressed or without address for stack examine again
        }
    } else {
        // It is a thread's data
        memory_blocks[tn] = std::vector<AMemoryBlock*>(1000, nullptr);
        int max_inst_addr = -1;

        for (int i = 1; i <= instruction_blocks.size(); i++) {
            int addr = instruction_blocks[i - 1].getAddress();

            if (addr > 500) throw std::runtime_error("Instruction address is out of bounds: " + std::to_string(addr));
            if (addr > max_inst_addr) max_inst_addr = addr;

            memory_blocks[tn][addr + 1] = new InstructionBlock(instruction_blocks[i - 1]); // For instruction section
        }

        int data_block_start = max_inst_addr + 2; // +1 for instruction section, +1 for data_block_start
        memory_blocks[tn][0] = new DataBlock(0, data_block_start);

        for (int i = 0; i < data_blocks.size(); i++) {
            int addr = data_blocks[i].getAddress();

            if (data_block_start + addr > 999) throw std::runtime_error("DataBlock address is out of bounds: " + std::to_string(addr));

            memory_blocks[tn][data_block_start + addr] = new DataBlock(data_blocks[i]); // For data section
        }

        for (int i = data_block_start; i < 1000; i++) {
            if (memory_blocks[tn][i] == nullptr) {
                memory_blocks[tn][i] = new DataBlock(i - data_block_start, 0);/* new DataBlock();  */ // For stack pointer 
            }
            // TODO: it could be addressed or without address for stack examine again
        }
        
        DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(os_blocks[0]);
        if (!data_block_pointer) {
            throw std::runtime_error("Data block pointer not found");
        }
        long start_of_data_section = data_block_pointer->getValue();

        if (os_blocks[start_of_data_section + tn + 1 + 21] != nullptr) {
            DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(os_blocks[start_of_data_section + tn + 1 + 21]);
            if (data_block_pointer) {
                long end_of_the_thread_table = data_block_pointer->getValue();
                DataBlock *os_pc = dynamic_cast<DataBlock*>(os_blocks[end_of_the_thread_table - 16]);
                DataBlock *memory_pc = dynamic_cast<DataBlock*>(memory_blocks[tn][data_block_start]);
                if (os_pc && memory_pc) {
                    os_pc->setValue(memory_pc->getValue());
                }
                DataBlock *os_sp = dynamic_cast<DataBlock*>(os_blocks[end_of_the_thread_table - 15]);
                DataBlock *memory_sp = dynamic_cast<DataBlock*>(memory_blocks[tn][data_block_start + 1]);
                if (os_sp && memory_sp) {
                    os_sp->setValue(memory_sp->getValue());
                }
            }
        }
    }

    instruction_blocks.clear();
    data_blocks.clear();
}

void Memory::printMemoryBlocks(int tn, long unsigned int start, long unsigned int end) {
    bool is_os = tn == 0 ? true : false;
    tn = tn - 1;

    if (is_os) {
        std::cout << "Registers: ";
        for (long unsigned int i = 0; i < 21; i++) {
            if (getRegister(i)) {
                std::cout << getRegister(i)->toShortString() << " ";
            }
        }
        std::cout << std::endl;
        for (long unsigned int i = start; i < os_blocks.size() && i < end; i++) {
            if (os_blocks[i]) {
                std::cout << os_blocks[i]->toString() << std::endl;
            }
        }
    } else {
        for (long unsigned int i = start; i < memory_blocks[tn].size() && i < end; i++) {
            if (memory_blocks[tn][i]) {
                std::cout << memory_blocks[tn][i]->toString() << std::endl;
            }
        }
    }
}

void Memory::debug() {
    int mem = 0;
    std::cerr << "******************************************************************************" << std::endl;
    std::cerr << "Debug Begin" << std::endl;
    std::cerr << "******************************************************************************" << std::endl;
    std::cerr << "---------------------------------" << std::endl;
    std::cerr << "OS Memory Begin" << std::endl;
    std::cerr << "---------------------------------" << std::endl;
    DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(os_blocks[0]);
    if (!data_block_pointer) {
        throw std::runtime_error("Data block pointer not found");
    }
    long start_of_data_section = data_block_pointer->getValue();
    long stack_pointer = getRegister(Memory::STACK_POINTER)->getValue();


    std::cerr << mem++ << "." << os_blocks[0]->toString() << " # Start of data section" << std::endl;
    for (long unsigned int i = 1; i < start_of_data_section; i++) {
        if (os_blocks[i]) {
            std::cerr << mem++ << "." << os_blocks[i]->toString() << std::endl;
        }
    }
    for (long unsigned int i = start_of_data_section; i < start_of_data_section + 21; i++) {
        if (os_blocks[i]) {
            std::cerr << mem++ << "." << os_blocks[i]->toString() << " # Register " << i - start_of_data_section << std::endl;
        }
    }
    for (long unsigned int i = start_of_data_section + 21; i < stack_pointer + start_of_data_section && i < os_blocks.size(); i++) {
        if (os_blocks[i]) {
            std::cerr << mem++ << "." << os_blocks[i]->toString() << std::endl;
        }
    }
    for (long unsigned int i = stack_pointer + start_of_data_section; i < os_blocks.size(); i++) {
        if (os_blocks[i]) {
            std::cerr << mem++ << "." << os_blocks[i]->toString() << " # Stack" << std::endl;
        }
    }

    std::cerr << "---------------------------------" << std::endl;
    std::cerr << "OS Memory End" << std::endl;
    std::cerr << "---------------------------------" << std::endl;

    for (int tn = 0; tn < 10; tn++) {
        std::cerr << "---------------------------------" << std::endl;
        std::cerr << "Thread " << tn + 1 << " Memory Begin" << std::endl;
        std::cerr << "---------------------------------" << std::endl;
        DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(memory_blocks[tn][0]);
        if (!data_block_pointer) {
            throw std::runtime_error("Data block pointer not found");
        }
        long start_of_data_section = data_block_pointer->getValue();
        long stack_pointer = getDataBlock(tn, 1)->getValue();


        std::cerr << mem++ << "." << memory_blocks[tn][0]->toString() << " # Start of data section" << std::endl;
        for (long unsigned int i = 1; i < start_of_data_section; i++) {
            if (memory_blocks[tn][i]) {
                std::cerr << mem++ << "." << memory_blocks[tn][i]->toString() << std::endl;
            }
        }
        for (long unsigned int i = start_of_data_section; i < stack_pointer + start_of_data_section; i++) {
            if (memory_blocks[tn][i]) {
                std::cerr << mem++ << "." << memory_blocks[tn][i]->toString() << std::endl;
            }
        }
        for (long unsigned int i = stack_pointer + start_of_data_section; i < memory_blocks[tn].size(); i++) {
            if (memory_blocks[tn][i]) {
                std::cerr << mem++ << "." << memory_blocks[tn][i]->toString() << " # Stack" << std::endl;
            }
        }
        std::cerr << "---------------------------------" << std::endl;
        std::cerr << "Thread " << tn + 1 << " Memory End" << std::endl;
        std::cerr << "---------------------------------" << std::endl;
        std::cerr << "******************************************************************************" << std::endl;
        std::cerr << "Debug End" << std::endl;
        std::cerr << "******************************************************************************" << std::endl;
        std::cerr << std::endl;
    }
}

void Memory::thread_table_debug() {
    int mem = 0;
    std::cerr << "******************************************************************************" << std::endl;
    std::cerr << "Debug Thread Table Begin" << std::endl;
    std::cerr << "******************************************************************************" << std::endl;

    DataBlock *data_block_pointer = dynamic_cast<DataBlock*>(os_blocks[0]);
    if (!data_block_pointer) {
        throw std::runtime_error("Data block pointer not found");
    }
    long start_of_data_section = data_block_pointer->getValue();

    int thread_table_ends[10] = {21, 22, 23, 24, 25, 26, 27, 28, 29, 30};

    for (int x = 0; x < 10; x++) {
        std::string name;
        if (x == 0) {
            name = "OS";
        } else {
            name = "Thread " + std::to_string(x);
        }
        std::cerr << "---------------------------------" << std::endl;
        std::cerr << "Thread Table For " << name << " Begin" << std::endl;
        std::cerr << "---------------------------------" << std::endl;

        DataBlock *end_of_thread_table = dynamic_cast<DataBlock*>(os_blocks[start_of_data_section + thread_table_ends[x]]);
        if (!end_of_thread_table) {
            throw std::runtime_error("Data block pointer not found");
        }
        long end_of_tt = end_of_thread_table->getValue();
        for (long unsigned int i = end_of_tt + start_of_data_section - 16 ; i <= end_of_tt + start_of_data_section && i < os_blocks.size(); i++) {
            if (os_blocks[i]) {
                std::cerr << mem++ << "." << os_blocks[i]->toString() << std::endl;
            }
        }

        std::cerr << "---------------------------------" << std::endl;
        std::cerr << "Thread Table For " << name << " End" << std::endl;
        std::cerr << "---------------------------------" << std::endl;

    }

    std::cerr << "******************************************************************************" << std::endl;
    std::cerr << "Debug Thread Table End" << std::endl;
    std::cerr << "******************************************************************************" << std::endl;
    std::cerr << std::endl;
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
    DataBlock *pg_reg = getRegister(Memory::PROGRAM_COUNTER); // Get first register from 0-20 of os_section
    if (!pg_reg) {
        throw std::runtime_error("Program counter register not found");
    }

    InstructionBlock *instruction_block = getInstructionBlock(tn, pg_reg->getValue()); 
    if (!instruction_block) {
        throw std::runtime_error("Instruction block not found");
    }

    return instruction_block;
}
