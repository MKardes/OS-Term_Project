#include "AMemoryBlock.h"

AMemoryBlock::AMemoryBlock(bool isInstruction)
    : isInstruction(isInstruction) {
}

bool AMemoryBlock::isInstructionBlock() const {
    return isInstruction;
}

/* MemoryBlock::MemoryBlock() {
    // TODO: This need to be handled
} */

// std::string MemoryBlock::toString() {
//     return "MemoryBlock";
// }