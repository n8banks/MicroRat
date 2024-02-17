#include <maze.hpp>


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
        int checkDirection();
        void constructMaze(string filename);
        void updateMouse();
        bool navigate(int possibleDirections[NUMSQUARES])

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

void Mouse :: constructMaze(string filename)
{
    // input for maze file
    ifstream ift;
    string line;
    string tempLine;

    // iterators, offset for row and col and the maze/mouse itself
    int i, j, offsetRow, offsetCol, startPosX, startPosY, startOrientation;

    // opens file using filename param
    ift.open(filename);

    startPosX = maze.startPosX; 
    startPosY = maze.startPosY;
    startOrientation = maze.startOrientation;
    offsetRow = maze.goalCoords[0][0]; 
    offsetCol = maze.goalCoords[0][1];
    for(i = 0; i < maze.row; i++)
    {
        // resets osC value after every iteration
        offsetCol = maze.goalCoords[0][1];

        // checks to see if past goal meaning osR needs to increase
        if(i > maze.goalCoords[0][0])
        {
            offsetRow = maze.goalCoords[2][0];
        }
        for(j = 0; j < maze.col; j++)
        {
            // checks to see if past goal meaning osC needs to increase
            if(j > maze.goalCoords[0][1])
            {
                offsetCol = maze.goalCoords[1][1];
            }
            // assigning values to each element of 2D array
            maze.mazeArray[i][j] = abs(offsetRow - i) + abs(offsetCol - j);
        }
    }

    currentRow = startPosX;
    currentCol = startPosY;
    currentOrientation = startOrientation;
    currentDistanceFromGoal = maze.mazeArray[currentRow][currentCol];
    // printing it out
    maze.printArray();
}

void Mouse :: updateMouse()
{
    prevRow = currentRow;
    prevCol = currentCol;
    prevDistanceFromGoal = currentDistanceFromGoal;

    if(cheeseWhore->currentOrientation == 1)
    {
        cheeseWhore->currentRow--;
    }
    else if(cheeseWhore->currentOrientation == 2)
    {
        cheeseWhore->currentCol++;
    }
    else if(cheeseWhore->currentOrientation == 3)
    {
        cheeseWhore->currentRow++;
    } 
    else if(cheeseWhore->currentOrientation == 4)
    {
        cheeseWhore->currentCol--;
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
        std :: cout << "startOrientation must be 1, 2, 3, or 4" << endl;
        return false;
    }

    newDirection = findLowestIndex(possibleDirections) + 1;

    if(cheeseWhore->currentOrientation < newDirection)
    {
        while(cheeseWhore->currentOrientation < newDirection)
        {
            cheeseWhore->currentOrientation++;
            updateMouse();
            mazeArray[cheeseWhore->currentRow][cheeseWhore->currentCol] = cheeseWhore->currentOrientation;
            mazeArray[cheeseWhore->prevRow][cheeseWhore->prevCol] = cheeseWhore->prevDistanceFromGoal;
            printArray();
        }
    }
    else if(cheeseWhore->currentOrientation > newDirection)
    {
        while(cheeseWhore->currentOrientation > newDirection)
        {
            cheeseWhore->currentOrientation--;
            updateMouse();
            mazeArray[cheeseWhore->currentRow][cheeseWhore->currentCol] = cheeseWhore->currentOrientation;
            mazeArray[cheeseWhore->prevRow][cheeseWhore->prevCol] = cheeseWhore->prevDistanceFromGoal;
            printArray();
        }
    }
    else if(cheeseWhore->currentOrientation == newDirection)
    {
        updateMouse();
        mazeArray[cheeseWhore->currentRow][cheeseWhore->currentCol] = cheeseWhore->currentOrientation;
        mazeArray[cheeseWhore->prevRow][cheeseWhore->prevCol] = cheeseWhore->prevDistanceFromGoal;
        printArray();
    }


    cheeseWhore->currentDistanceFromGoal = possibleDirections[newDirection-1];
    
    if(cheeseWhore->currentDistanceFromGoal == 0)
    {
        cheeseWhore->atGoal = true;
    }

    return true;
}

void Mouse :: findSurroundingValues(int surroundingSquares[NUMSQUARES])
{
    int x = 1, y = 1, i = 0;

    if(currentRow - y < row && currentRow - y >=0)
    {
        surroundingSquares[i++] = mazeArray[currentRow - y][currentCol];
    }
    else
    {
        surroundingSquares[i++] = INT_MAX;
    }
    if(currentCol + x < col && cheeseWhore->currentCol >= 0)
    {
        surroundingSquares[i++] = mazeArray[cheeseWhore->currentRow][cheeseWhore->currentCol + x];
    }
    else
    {
        surroundingSquares[i++] = INT_MAX;
    }
    if(cheeseWhore->currentRow + y >= 0 && cheeseWhore->currentRow + y < row)
    {
        surroundingSquares[i++] = mazeArray[cheeseWhore->currentRow + y][cheeseWhore->currentCol];
    }
    else
    {
        surroundingSquares[i++] = INT_MAX;
    }
    if(cheeseWhore->currentCol -x >= 0 && cheeseWhore->currentCol - x < col)
    {
        surroundingSquares[i++] = mazeArray[cheeseWhore->currentRow][cheeseWhore->currentCol - x];
    }
    else
    {
        surroundingSquares[i++] = INT_MAX;
    }
}

bool Maze :: Update(int **floodArray)
{
    if(mouse)
}


int main(int argc, char **argv)
{
    float number = 0.0;
    int i, potentialValues[NUMSQUARES];
    bool navigateFlag = true;
    string array;
    std :: cout << "Loading maze: \n";
    std :: cout << std::setprecision(2) << std::fixed;
    while(number < 100)
    {
        std :: cout << '\r' << flush;
        std :: cout << number;
        number+= 0.00005; 
    }

    Maze test = Maze(argv[1]);
    
    // std :: cout << "Proper input: ./a.out + 'filename.txt'\n";

    array = test.printArray();
    cout << array;
    while(!test.cheeseWhore->atGoal && navigateFlag)
    {
        number = 0.0;
        test.findSurroundingValues(potentialValues);
        navigateFlag = test.navigate(potentialValues);

        while(number < 100)
        {
            std :: cout << '\r' << flush;
            std :: cout << number;
            number+= 0.00005; 
        }
        
        array = test.printArray();
        cout << array;
    }
    return 0;
}
