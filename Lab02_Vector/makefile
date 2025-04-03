# Executable name
TARGET = Lab5
# Python grading script
GRADER = GradingScript.py
# Default ID
ID ?= Personal

# Default rule
all: test

# Rule to build the executable
$(TARGET): MainTest.cpp
	@echo
	@echo Compiling...
	@g++ -std=c++11 MainTest.cpp -o $(TARGET)
	@echo

# Rule to run the program
run: $(TARGET)
	@echo
	@echo Running Program...
	@./$(TARGET) ./Inputs/input_$(ID).txt >result_$(ID).txt
	@echo

# Rule to compare output
test: run
	@echo
	@echo Comparing Output...
	@python3 $(GRADER) result_$(ID).txt ./Outputs/output_$(ID).txt
	@echo

# 
test_all: $(TARGET)
	@echo
	@for id in $$(seq 0 9); do \
		echo "Running Test for ID=$$id..."; \
		./$(TARGET) ./Inputs/input_$$id.txt > result_$$id.txt; \
		echo "Comparing Output for ID=$$id..."; \
		python3 $(GRADER) result_$$id.txt ./Outputs/output_$$id.txt; \
		echo ""; \
	done
	@echo

# Clean rule
clean:
	@echo
	@echo Cleaning...
	@rm -f $(TARGET) result_*.txt;
	@echo
