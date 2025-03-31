1: Compile MainTest to test data files
g++ -std=c++11 MainTest.cpp -o Lab1

2: Running the testing program
./Lab1 ./Inputs/input_0.txt >result_0.txt

3: Comparing your result with expected output
python3 GradingScript.py result_0.txt ./Outputs/output_0.txt

If you see "Yes", then your program is correct. Or if you see "No", your program is incorrect.