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

typedef struct node
{
    int direction;
    struct node *next;
    struct node *prev;
}node;

typedef struct optimalPathList
{
    node *head;
    node *tail;
    int count;
}optimalPathList;

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

        string printArray();
        void update(int r, int c, int newVal);


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
    int i, j, offsetRow, offsetCol;

    // opens file using filename param
    ift.open(filename);

    // actually taking input
    ift >> row >> col;

    cout << row << ", " << col;

    // allocating memory to mazeArr (for variable sizes)
    mazeArray = new int*[row];
    for(i = 0; i < row; i++)
    {
        mazeArray[i] = new int[col];
    }

    floodArray = new int*[row + 2];
    for(i = 0; i < row + 2; i++)
    {
        floodArray[i] = new int[col + 2];
        for(j = 0; j < col + 2; j++)
        {
            floodArray[i][j] = 0;
        }
    }

    for(i = 0; i < col + 2; i++)
    {
        floodArray[0][i] = - 1;
        floodArray[row + 2][i] = - 1;
    }
    for(i = 1; i < row + 1; i++)
    {
        floodArray[i][0] = - 1;
        floodArray[i][col + 2] = - 1;
    } 
    ift >> startPosX >> startPosY >> startOrientation;

    cout << startPosX << ", " << startPosY << ", " << startOrientation;

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

void Maze :: update(int r, int c, int newVal)
{
    mazeArray[r][c] = newVal;
}

node *createNode(int direction, node *prev)
{
    node *newNode = (node*)malloc(sizeof(node));
    
    if(prev != NULL)
        newNode->next = prev->next;
    else
        newNode->next = NULL;

    newNode->prev = prev;
    newNode->direction = direction;
    cout << " NODE MADE! :D\n";
    return newNode;
}

void destroyNode(node *victim)
{
    if(victim->next != NULL)
    {
        destroyNode(victim->next);
    }
    victim->prev->next = NULL;
    free(victim);
}

optimalPathList* createList(int startValue)
{
    optimalPathList *newList = (optimalPathList*)malloc(sizeof(optimalPathList));
    newList->head = createNode(startValue, NULL);
    newList->tail = newList->head;
    newList->count = 1;
    return newList;
}

void insertNode(int direction, optimalPathList *list)
{
    node *temp = list->tail;
    list->tail->next = createNode(direction, temp);
    list->tail = temp->next;
    list->count++;
}

void printList(optimalPathList *list)
{
    node *temp = list->head;
    int i;

    for(i = 0; i < list->count; i++)
    {
        cout << temp->direction << " ";
        temp = temp->next;
    }
}