#ifndef MEMORY_BLOCK_H
# define MEMORY_BLOCK_H

# include <iostream>
# include <string>

class AMemoryBlock {
protected:
    bool isInstruction;

public:
    AMemoryBlock(bool isInstruction);
    /* MemoryBlock(); */
    virtual ~AMemoryBlock() = default;    // Virtual destructor
    bool isInstructionBlock() const;
    
    // Pure virtual functions that derived classes must implement
    virtual std::string toString() const = 0;
};

#endif