# Executable name
TARGET = Lab3
# Source file
SRC = MainTest.cpp
# Input and output files
INPUT = ./Inputs/input_$(ID).txt
RESULT = result_$(ID).txt
EXPECTED_OUTPUT = ./Outputs/output_$(ID).txt
# Python grading script
GRADER = GradingScript.py
# Default ID
ID ?= 0

# Default rule
all: test

# Rule to build the executable
$(TARGET): $(SRC)
	echo "\nCompiling...\n"
	g++ -std=c++11 $(SRC) -o $(TARGET)

# Rule to run the program
run: $(TARGET)
	echo "\nRuning Program...\n"
	./$(TARGET) $(INPUT) >$(RESULT)

# Rule to compare output
test:
	echo "\nComparing Output...\n"
	python3 $(GRADER) $(RESULT) $(EXPECTED_OUTPUT)

# Clean rule
clean:
	echo "\nCleaning...\n"
	rm -f $(TARGET) $(RESULT)
