# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++11
# Executable name
TARGET = Lab2
# Source file
SRC = MainTest.cpp
# Input and output files
INPUT = ./Inputs/input_$(ID).txt
RESULT = result_$(ID).txt
EXPECTED_OUTPUT = ./Outputs/output_$(ID).txt
# Python grading script
GRADER = python3 GradingScript.py
# Default ID
ID ?= 0

# Default rule: Compile the program
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Rule to run the program
run: $(TARGET)
	./$(TARGET) $(INPUT) >$(RESULT)

# Rule to compare output
test: run
	$(GRADER) $(RESULT) $(EXPECTED_OUTPUT)

# Clean rule
clean:
	rm -f $(TARGET) $(RESULT)
