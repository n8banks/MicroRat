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
        bool wallLeft;
        bool wallRight;
        bool wallFront;
        bool wallBack;
        Maze maze;

        Mouse(string filename);
        int checkDirection();
        void findSurroundingValues(int surroundingSquares[NUMSQUARES]);
        void updateMouse();
        void updateWalls();
        void move(int direction);
        bool isValid();
        bool optimalPathBackTrack();

        optimalPathList *path;
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
    path = createList(currentOrientation);
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
void Mouse :: updateWalls()
{
    if(wallFront && wallLeft && wallRight)
    {
        if(currentOrientation == 1)
        {
            maze.floodArray[currentRow][currentCol] = 13;
        }
        else if(currentOrientation == 2)
        {
            maze.floodArray[currentRow][currentCol] = 12;
        }
        else if(currentOrientation == 3)
        {
            maze.floodArray[currentRow][currentCol] = 11;
        }
        else if(currentOrientation == 4)
        {
            maze.floodArray[currentRow][currentCol] = 14;
        }
    }
    else if(wallFront && wallLeft && !wallRight)
    {
        if(currentOrientation == 1)
        {
            maze.floodArray[currentRow][currentCol] = 8;
        }
        else if(currentOrientation == 2)
        {
            maze.floodArray[currentRow][currentCol] = 7;
        }
        else if(currentOrientation == 3)
        {
            maze.floodArray[currentRow][currentCol] = 6;
        }
        else if(currentOrientation == 4)
        {
            maze.floodArray[currentRow][currentCol] = 5;
        }
    }
    else if(wallFront && wallRight && !wallLeft)
    {
        if(currentOrientation == 1)
        {
            maze.floodArray[currentRow][currentCol] = 7;
        }
        else if(currentOrientation == 2)
        {
            maze.floodArray[currentRow][currentCol] = 6;
        }
        else if(currentOrientation == 3)
        {
            maze.floodArray[currentRow][currentCol] = 5;
        }
        else if(currentOrientation == 4)
        {
            maze.floodArray[currentRow][currentCol] = 8;
        }
    }
    else if(wallLeft && wallRight && !wallFront)
    { 
        if(currentOrientation == 1 || currentOrientation == 3)
        {
            maze.floodArray[currentRow][currentCol] = 9;
        }
        else if(currentOrientation == 2 || currentOrientation == 4)
        {
            maze.floodArray[currentRow][currentCol] = 10;
        }
    }
    else if(wallFront)
    {
        if(currentOrientation == 1)
        {
            maze.floodArray[currentRow][currentCol] = 2;
        }
        else if(currentOrientation == 2)
        {
            maze.floodArray[currentRow][currentCol] = 3;
        }
        else if(currentOrientation == 3)
        {
            maze.floodArray[currentRow][currentCol] = 4;
        }
        else if(currentOrientation == 4)
        {
            maze.floodArray[currentRow][currentCol] = 1;
        }
    }
    else if(wallLeft)
    {
        if(currentOrientation == 1)
        {
            maze.floodArray[currentRow][currentCol] = 1;
        }
        else if(currentOrientation == 2)
        {
            maze.floodArray[currentRow][currentCol] = 2;
        }
        else if(currentOrientation == 3)
        {
            maze.floodArray[currentRow][currentCol] = 3;
        }
        else if(currentOrientation == 4)
        {
            maze.floodArray[currentRow][currentCol] = 4;
        }
    }
    else if(wallRight)
    {
        if(currentOrientation == 1)
        {
            maze.floodArray[currentRow][currentCol] = 3;
        }
        else if(currentOrientation == 2)
        {
            maze.floodArray[currentRow][currentCol] = 4;
        }
        else if(currentOrientation == 3)
        {
            maze.floodArray[currentRow][currentCol] = 1;
        }
        else if(currentOrientation == 4)
        {
            maze.floodArray[currentRow][currentCol] = 2;
        }
    }
    else if(!wallFront && !wallLeft && !wallRight)
    {
        maze.floodArray[currentRow][currentCol] = 15;
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

bool Mouse :: isValid()
{
    int check = maze.floodArray[currentRow][currentCol];
    
    if(check == -1)
    {
        return false;
    }
    else if(currentOrientation == 1)
    {
        if(check == 4 || check == 5 || check == 6 || check == 10 || check == 12 || check == 14 || check == 16)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else if(currentOrientation == 2)
    {
        if(check == 1 || check == 5 || check == 8 || check == 9 || check == 11 || check == 13 || check == 14 || check == 16)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else if(currentOrientation == 3)
    {
        if(check == 2 || check == 7 || check == 8 || check == 10 || check == 12 || check == 13 || check == 14 || check == 16)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else if(currentOrientation == 4)
    {
        if(check == 3 || check == 6 || check == 9 || check == 11 || check == 12 || check == 13 || check == 16)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}
// possible orientations: 1, 2, 3 ,4 meaning: 1: ^, 2: ->, 3: V, 4: <-


void Mouse :: move(int direction)
{
    
}
bool Mouse :: optimalPathBackTrack()
{
    int i;
    if(atGoal)
    {
        return true;
    }
    else 
    {
        for(i = 0; i < 4; i++)
        {
            if(isValid())
            {
                move(currentOrientation);
                if(optimalPathBackTrack())
                {
                    return true;
                }
            }
            else
            {
                currentOrientation = currentOrientation % 4 + 1;
                return optimalPathBackTrack();
            }
        }
    }
    return false;
}


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
        //navigateFlag = rat.navigate(potentialValues);

        while(number < 100)
        {
            std :: cout << '\r' << flush;
            std :: cout << number;
            number+= 0.00005; 
        }
        
        array = rat.maze.printArray();
        cout << array;
    }

    optimalPathList *path = createList(0);
    insertNode(1, path);
    insertNode(2,path);
    printList(path);
    return 0;
}
