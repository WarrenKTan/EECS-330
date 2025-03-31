1: Compile MainTest to test data files
"g++ -std=c++11 MainTest.cpp -o Lab2"

2: Running the testing program
"./Lab2 input.txt >result.txt"

3: Comparing your result with expected output
"python3 GradingScript.py result.txt output.txt"
If you see "Yes", then your program is correct. Or if you see "No", your program is incorrect.