# Executable name
TARGET = Lab9.exe
# Python grading script
GRADER = GradingScript.py
# Default ID
ID ?= Personal
SIZE ?= 5 5

# Default rule
all: run

# Rule to build the executable
$(TARGET): MainTest.cpp
	@echo
	@echo Compiling...
	@g++ -std=c++11 MainTest.cpp -o $(TARGET)

# Rule to run the program
run: $(TARGET)
	@echo
	@echo Running Program...
	@echo ./$(TARGET) $(SIZE)
	@./$(TARGET) $(SIZE)
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
	@mkdir -p Results
	@for sz in "5 5" "5 10" "10 10" "10 15" "15 15" "20 20" "20 40" "30 30" "30 50"; do \
		echo "Running Test for SIZE=$$sz..."; \
		./$(TARGET) $$sz >./Results/result_$$(echo $$sz | tr ' ' 'x').txt; \
		echo ""; \
	done
	@echo


# Clean rule
clean:
	@echo
	@echo Cleaning...
	@rm -f $(TARGET) ./Results/result_*.txt;
	@echo
