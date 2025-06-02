# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -g -I include

# Source files
SRCS = src/main.cpp src/CPU.cpp src/Memory.cpp src/AMemoryBlock.cpp src/DataBlock.cpp src/InstructionBlock.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = cpu_simulator

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compilation rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dependencies
CPU.o: src/CPU.cpp include/CPU.h include/Memory.h
Memory.o: src/Memory.cpp include/Memory.h include/AMemoryBlock.h include/DataBlock.h include/InstructionBlock.h
AMemoryBlock.o: src/AMemoryBlock.cpp include/AMemoryBlock.h
DataBlock.o: src/DataBlock.cpp include/DataBlock.h include/AMemoryBlock.h
InstructionBlock.o: src/InstructionBlock.cpp include/InstructionBlock.h include/AMemoryBlock.h
main.o: src/main.cpp include/CPU.h

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)

# Run target
run: $(TARGET)
	./$(TARGET)

re: clean all

rerun: clean all run

.PHONY: all clean run
