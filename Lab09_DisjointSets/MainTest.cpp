#include <iostream>
#include <cstdlib>
#include <string>

#include "MyMaze_w125t659.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "Please run the program with two parameters: [num_rows] [num_columns]." << endl;
        cout << "e.g.: \"./a.out 10 20\"." << endl;
        exit(1);
    }

    int nRows = 0, nCols = 0;
    nRows = stoi(string(argv[1]));
    nCols = stoi(string(argv[2]));
    if(nRows > 0 && nCols > 0)
    {
        srand(time(0));      
        MyMaze maze(nRows, nCols);
        maze.buildMaze();
        maze.printMaze();
        return 0;
    }
    else
    {
        cout << "Please run the program with two parameters: [num_rows] [num_columns]." << endl;
        cout << "Both parameters need to be larger than 0." << endl;
        cout << "e.g.: \"./a.out 10 20\"." << endl;
        return 1;
    }
}

