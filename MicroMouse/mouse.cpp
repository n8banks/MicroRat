#include "maze.hpp"


using namespace std;

class Mouse
{
    public:
        int currentRow;
        int currentCol;
        int prevRow;
        int prevCol;
        int currentOrientation;
        int currentDistanceFromGoal;
        int prevDistanceFromGoal;
        bool atGoal;

        Maze maze;

        Mouse(string filename);
        int checkDirection();
        void findSurroundingValues(int surroundingSquares[NUMSQUARES]);
        void updateMouse();
        bool navigate(int possibleDirections[NUMSQUARES]);

};

Mouse :: Mouse(string filename) : maze(filename)
{
    // input for maze file

    // iterators, offset for row and col and the maze/mouse itself
  
    currentRow = maze.startPosX;
    currentCol = maze.startPosY;
    currentOrientation = maze.startOrientation;
    std :: cout << "HI im in the constructor:" << maze.startOrientation << "start x: " << maze.startPosX << "start y: " << maze.startPosY<< endl;
    currentDistanceFromGoal = maze.mazeArray[currentRow][currentCol];
    
    // printing it out
    maze.printArray();
}

void Mouse :: updateMouse()
{
    prevRow = currentRow;
    prevCol = currentCol;
    prevDistanceFromGoal = currentDistanceFromGoal;

    if(currentOrientation == 1)
    {
        currentRow--;
    }
    else if(currentOrientation == 2)
    {
        currentCol++;
    }
    else if(currentOrientation == 3)
    {
        currentRow++;
    } 
    else if(currentOrientation == 4)
    {
        currentCol--;
    }
}

// when passing an array to function, it passes pointer to first element (relic of C)
// need to pass array by reference 
int findLowestIndex(int possibleDirections[NUMSQUARES])
{
    int i, currentLowest = INT_MAX, retval; 
    for(i = 0; i < NUMSQUARES; i++)
    {
        if(possibleDirections[i] < currentLowest)
        {
            retval = i;
            currentLowest = possibleDirections[i];
        }
    }

    return retval;
}

// possible orientations: 1, 2, 3 ,4 meaning: 1: ^, 2: ->, 3: V, 4: <-
bool Mouse :: navigate(int possibleDirections[NUMSQUARES])
{
    int newDirection, x, y, i, j;
    if(currentOrientation > 4 || currentOrientation < 1)
    {
        std :: cout << "startOrientation must be 1, 2, 3, or 4, it is currently:" << maze.startOrientation << endl;
        return false;
    }

    newDirection = findLowestIndex(possibleDirections) + 1;

    if(currentOrientation < newDirection)
    {
        while(currentOrientation < newDirection)
        {
            currentOrientation++;
            updateMouse();
            maze.mazeArray[currentRow][currentCol] = currentOrientation;
            maze.mazeArray[prevRow][prevCol] = prevDistanceFromGoal;
            maze.printArray();
        }
    }
    else if(currentOrientation > newDirection)
    {
        while(currentOrientation > newDirection)
        {
            currentOrientation--;
            updateMouse();
            maze.mazeArray[currentRow][currentCol] = currentOrientation;
            maze.mazeArray[prevRow][prevCol] = prevDistanceFromGoal;
            maze.printArray();
        }
    }
    else if(currentOrientation == newDirection)
    {
        updateMouse();
        maze.mazeArray[currentRow][currentCol] = currentOrientation;
        maze.mazeArray[prevRow][prevCol] = prevDistanceFromGoal;
        maze.printArray();
    }


    currentDistanceFromGoal = possibleDirections[newDirection-1];
    
    if(currentDistanceFromGoal == 0)
    {
        atGoal = true;
    }

    return true;
}

void Mouse :: findSurroundingValues(int surroundingSquares[NUMSQUARES])
{
    int x = 1, y = 1, i = 0;

    if(currentRow - y < maze.row && currentRow - y >=0)
    {
        surroundingSquares[i++] = maze.mazeArray[currentRow - y][currentCol];
    }
    else
    {
        surroundingSquares[i++] = INT_MAX;
    }
    if(currentCol + x < maze.col && currentCol >= 0)
    {
        surroundingSquares[i++] = maze.mazeArray[currentRow][currentCol + x];
    }
    else
    {
        surroundingSquares[i++] = INT_MAX;
    }
    if(currentRow + y >= 0 && currentRow + y < maze.row)
    {
        surroundingSquares[i++] = maze.mazeArray[currentRow + y][currentCol];
    }
    else
    {
        surroundingSquares[i++] = INT_MAX;
    }
    if(currentCol -x >= 0 && currentCol - x < maze.col)
    {
        surroundingSquares[i++] = maze.mazeArray[currentRow][currentCol - x];
    }
    else
    {
        surroundingSquares[i++] = INT_MAX;
    }
}

// bool Maze :: Update(int **floodArray)
// {
//     if(mouse)
// }


int main(int argc, char **argv)
{
    float number = 0.0;
    int i, potentialValues[NUMSQUARES];
    bool navigateFlag = true;
    string array;

    if(argv[1] == NULL)
    {
        cout << "proper input ./a.out filename.txt" << endl;
        return 1;
    }

    Mouse rat = Mouse(argv[1]);

    
    std :: cout << "Loading maze: \n";
    std :: cout << std::setprecision(2) << std::fixed;
    while(number < 100)
    {
        std :: cout << '\r' << flush;
        std :: cout << number;
        number+= 0.00005; 
    }
    
    array = rat.maze.printArray();
    cout << array;
    while(!rat.atGoal && navigateFlag)
    {
        number = 0.0;
        rat.findSurroundingValues(potentialValues);
        navigateFlag = rat.navigate(potentialValues);

        while(number < 100)
        {
            std :: cout << '\r' << flush;
            std :: cout << number;
            number+= 0.00005; 
        }
        
        array = rat.maze.printArray();
        cout << array;
    }
    return 0;
}
