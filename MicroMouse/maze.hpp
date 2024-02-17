#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits.h>
#include <sstream>
#include <string>
#include <curses.h>

#define NUMSQUARES 4
// define POSSIBLEWALLS[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}

using namespace std;


class Maze
{
    public: 
        int row;
        int col;
        // stored as x,y  0 1
        //                2 3
        int goalCoords[4][2];
        int startPosX;
        int startPosY;
        int startOrientation;
        int **mazeArray;
        int **floodArray;

        bool Update(int **floodArray);
        bool navigate(int possibleDirections[NUMSQUARES]);
        string printArray();
        void findSurroundingValues(int surroundingSquares[NUMSQUARES]);
        void updateMouse();

        Maze(string filename);
        
};

string Maze :: printArray()
{
    int i, j;
    float number = 0;
    std :: ostringstream retval;
    string end;
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
        {
            retval << std::right << std::setw(4) << mazeArray[i][j];
        }
        end = i == row - 1 ? "" : "\n"; 
        retval << end << end;
    }
    return retval.str();
}

Maze :: Maze(string filename)
{
    // input for maze file
    ifstream ift;
    string line;
    string tempLine;

    // iterators, offset for row and col and the maze/mouse itself
    int i, j, offsetRow, offsetCol, startPosX, startPosY, startOrientation;

    // opens file using filename param
    ift.open(filename);

    // actually taking input
    ift >> row >> col;

    // allocating memory to mazeArr (for variable sizes)
    mazeArray = new int*[row];
    for(i = 0; i < row; i++)
    {
        mazeArray[i] = new int[col];
    }

    floodArray = new int*[row];
    for(i = 0; i < row; i++)
    {
        floodArray[i] = new int[col];
        for(j = 0; j < col; j++)
        {
            floodArray[i][j] = 0;
        }
    }
    ift >> startPosX >> startPosY >> startOrientation;

    // taking in coords of the goal (for variable goal locations)
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 2; j++)
        {
            ift >> goalCoords[i][j];
        }
    }

    // sets osR + osC = top Left most goal coords
    offsetRow = goalCoords[0][0]; 
    offsetCol = goalCoords[0][1];
    for(i = 0; i < row; i++)
    {
        // resets osC value after every iteration
        offsetCol = goalCoords[0][1];

        // checks to see if past goal meaning osR needs to increase
        if(i > goalCoords[0][0])
        {
            offsetRow = goalCoords[2][0];
        }
        for(j = 0; j < col; j++)
        {
            // checks to see if past goal meaning osC needs to increase
            if(j > goalCoords[0][1])
            {
                offsetCol = goalCoords[1][1];
            }
            // assigning values to each element of 2D array
            mazeArray[i][j] = abs(offsetRow - i) + abs(offsetCol - j);
        }
    }
    // printing it out
    printArray();
}

// bool Maze :: Update(int **floodArray)
// {
//     if(mouse)
// }