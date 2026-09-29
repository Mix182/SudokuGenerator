#include <stdio.h>
#include <stdlib.h>


/*
    the grid is stored as a list of 81 unsigned short ints with:
        - 1-9 representing numbers
        - 0 representing an empty spot
    ex:
    [1,0,9,0,2,0,4,6,3,4,3,7,1,9,6,2,8,5,6,2,8,3,5,4,7,9,1,7,8,5,6,1,2,3,4,9,9,1,4,7,3,5,8,2,6,3,6,2,4,8,9,5,1,7,2,9,6,5,4,3,1,7,8,8,7,3,2,6,1,9,5,4,5,4,1,9,7,8,6,3,2]

    represents the grid:
    1 9| 2 |463
    437|196|285
    628|354|791
    ---|---|---
    785|612|349
    914|735|826
    362|489|517
    ---|---|---
    296|543|178
    873|261|954
    541|978|632




    00 1 1001 111111111 - each cell (an unsigned short int), when uncompleted
    ^  ^ ^    ^ which options are avaliable (ex: 001000001 = only 7 and 1 can be placed here)
    |  | L how many unused options are avaliable (reduces compute time)
    |  L Whether this is a completed cell or an open one (1 = open, 0 = closed)
    L unused

    000000000000 1001 - each cell when completed
    ^            ^ the number at said position
    L unused

    


    the algorithm:
    1. We fill the diagonal 3x3 squares with numbers (they do not effect each other)
       159|   |   
       437|   |   
       628|   |   
       ---|---|---
          |612|   
          |735|   
          |489|   
       ---|---|---
          |   |178
          |   |954
          |   |632

    2. We duplicate the above grid and try to fill it (an 'attempt'):
       2.1. We find a random cell with the fewest number of possibilities and 'complete' it by choosing one of them
            - if the fewest number of possibilities is 0, we abort this 'attempt' and go back to the 1/3 filled board from step 1
       2.2. We repeat step 2.1 untill the board is filled

    3. We remove 'empty' (variable provided to the function) numbers at random (the worst part of this algorithm, as there is no guaranteee of only one solution)

*/

typedef unsigned short int usint;

usint* generateSudoku(int empty); // Generates a comletable (not neccessarily one possibility) sudoaku with 'empty' numbers missing

usint* getPrefilledBoard(); // Fills the three diagonal 3x3 squares with numbers
int removePossibility(usint* board, int pos, int num); // Updates a single cell, returns 0 if this cell now has 0 possibilities
int removePossibilities(usint* board, int pos); // Updates uncompleted numbers affected by number at 'pos' to reflect their possible states, returns 1 if all went well, returns 0 if any number lost all options
usint* copyBoard(usint* board); // Returns a pointer to a copy of the specified board
int fillAttempt(usint* attempt); // Tries to fill a board, returns 1 if successful, returns 0 otherwise
void removeNumbers(usint* board, int empty); // Removes 'empty' numbers from the board

void printBoard(usint* board); // Prints the specified sudoku board
