#ifndef _MYMAZE_H_
#define _MYMAZE_H_

#include <iostream>
#include <cstdlib>

#include "MyDisjointSets_w125t659.h"

class MyMaze 
{
  public:
	
    // constructor
	explicit MyMaze(const size_t r, const size_t c)
    : numRows(r), numCols(c), dSets(r * c)
	{
        // close all leftRightWalls
        leftRightWalls = new bool*[numRows];
        for(size_t i = 0; i < numRows; ++i){
            leftRightWalls[i] = new bool[numCols];
            for(size_t j = 0; j < numCols; ++j){
                leftRightWalls[i][j] = true;
            }
        }

        // close all topDownWalls
        topDownWalls = new bool*[numRows];
        for(size_t i = 0; i < numRows; ++i){
            topDownWalls[i] = new bool[numCols];
            for(size_t j = 0; j < numCols; ++j){
                topDownWalls[i][j] = true;
            }
        }

        // remove entrance and exit walls
        topDownWalls[0][0] = false;
	}

    // destructor
    ~MyMaze(void)
    {
        for(size_t i = 0; i < numRows; ++i){
            delete[] leftRightWalls[i];
            delete[] topDownWalls[i];
        }
        delete[] leftRightWalls;
        delete[] topDownWalls;
    }

    // builds a random maze
    // the top wall of the first cell (the top-left corner) should be open as the entrance
    // the bottom wall of the last cell (the bottom-right corner) should be open as the exit
    // algorithm hint: keep breaking walls until the entrance and exit become connected
	void buildMaze(void)
	{
        // numbers contained in the entrance and exit cells
        size_t entrance = 0;
        size_t exit = numRows * numCols - 1;

        while(dSets.find(entrance) != dSets.find(exit)){
            // picks a random wall from all walls available
            size_t random = rand();

            if(rand() % 2 == 1){ // leftRightWalls (LR)
                // inner columns in LR
                size_t col = (rand() % (numCols - 1)) + 1;
                
                // rows in LR
                size_t row = rand() % numRows;

                // break wall
                if(leftRightWalls[row][col]){
                    // numbers contained in the left and right cells
                    int rightCell = ((row * numCols) + col);
                    int leftCell = ((row * numCols) + col) - 1;

                    // std::cout << "LRwall: " << leftCell << " " << rightCell << std::endl;
                    if (dSets.find(rightCell) != dSets.find(leftCell)){
                        dSets.unionSets(rightCell, leftCell);
                        leftRightWalls[row][col] = false;
                        // printMaze();
                        // std::cout << std::endl << std::endl;
                    }
                }
            }else{ // topDownWalls (TD)
                // columns in TD
                size_t col = rand() % numCols;
                
                // inner rows in TD
                size_t row = (rand() % (numRows - 1)) + 1;
                
                // break wall
                if(topDownWalls[row][col]){
                    // numbers contained in the top and bottom cells
                    int topCell = ((row * numCols) + col);
                    int bottomCell = ((row * numCols) + col) - numCols;

                    // std::cout << "TDwall: " << topCell << " " << bottomCell << std::endl;
                    if (dSets.find(topCell) != dSets.find(bottomCell)){
                        dSets.unionSets(topCell, bottomCell);
                        topDownWalls[row][col] = false;
                        // printMaze();
                        // std::cout << std::endl << std::endl;
                    }
                }
            }
        }
	}

    // prints the maze
    // for top-down walls you can use '-' (ASCII code 45)
    // for left-right walls you can use '|' (ASCII code 124)
    // feel free to try out other symbols if your system supports extended ASCII and/or UTF-8
    void printMaze(std::ostream& out = std::cout)
    {
        // chooses which array to pick from during print
        bool isLeftRightWall = false;
        size_t row = 0;
        size_t cellWidth = 1;
        bool plusCorners = false;
        
        std::string spaces(cellWidth, ' ');
        std::string dashes(cellWidth, '-');
        const char* corners = (plusCorners) ? "+" : " ";

        while(row < numRows){
            if(isLeftRightWall){
                // leftRightWalls
                for(size_t col = 0; col < numCols; ++col){
                    out << ((leftRightWalls[row][col]) ? "|" : " ") << spaces;
                }
                out << "|";
                row += 1;
            }else{
                // topBottomWalls
                out << corners;
                for(size_t col = 0; col < numCols; ++col){
                    out << ((topDownWalls[row][col]) ? dashes : spaces) << corners;
                }
            }
            out << std::endl;
            isLeftRightWall = !isLeftRightWall;
        }

        // print bottom
        out << corners;
        for(size_t col = 0; col < numCols - 1; ++col){
            out << dashes << corners;
        }
        out << spaces << corners;
    }

  private:
	size_t numRows = 0;         // the number of rows of the maze
	size_t numCols = 0;         // the number of columns of the maze
    MyDisjointSets dSets;       // the disjoint set class that keeps track of maze cell connection
    bool** leftRightWalls;      // the 2D array that indicates whether walls between left-right neighbors present
    bool** topDownWalls;        // the 2D array that indicates whether walls between the top-down neighbors preset

};


#endif // __MYMAZE_H__