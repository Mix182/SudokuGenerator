#include "sudokuGenerator.h"
#include <string.h>

void printBoard(usint* board) {
    // very crude, couldn't be bothered
    int row = 0;
    for (int i = 0; i < 11; i++) {
        if (i == 3 || i == 7) printf("-----|-----|-----\n");
        else {
            printf("%d %d %d|%d %d %d|%d %d %d\n", board[9*row], board[9*row+1], board[9*row+2], board[9*row+3], board[9*row+4], board[9*row+5], board[9*row+6], board[9*row+7], board[9*row+8]);

            row++;
        }
    }
}

int removePossibility(usint* board, int pos, int num) {
    if (board[pos] < 0b0010000000000000) return 1; // already complete number
    
    if ((board[pos] & (1 << (num-1))) == 0) return 1; // already not an option

    board[pos] ^= (1 << (num-1)); // remove num from options

    board[pos] -= (1 << 9); // subtract one from the counter

    if ((board[pos] & 0b0001111000000000) == 0) return 0; // there are no more options
                                  
    return 1;

}

int removePossibilities(usint* board, int pos) {
    int num = board[pos];

    for (int i = 0; i < 9; i++) {
        if (!removePossibility(board, 9 * (pos / 9) + i, num)) return 0; // row

        if (!removePossibility(board, 9 * i + (pos % 9), num)) return 0; // col

        if (!removePossibility(board, 9 * (3*(pos/27)+(i/3)) + (3*((pos%9)/3)+(i%3)), num)) return 0; // 3x3 square
        // top left of 3x3 square
        // |                     position within square
        // V                     V
        // 3 * (pos / 27)      + (i / 3) - y positions
        // 3 * ((pos % 9) / 3) + (i % 3) - x position
    }

    return 1;
}

usint* getPrefilledBoard() {
    usint* board = malloc(81 * sizeof(usint));

    for (int i = 0; i < 81; i ++) board[i] = 0b0011001111111111;

    int num_i;
    int num;
    usint avaliable[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 3; i++) {
        for (int spot = 0; spot < 9; spot++) {
            num_i = rand() % (9 - spot);
            num = avaliable[num_i];

            board[9*(i*3+spot/3) + (i*3+spot%3)] = num;
            if (!removePossibilities(board, 9*(i*3+spot/3) + (i*3+spot%3))) {
                printf("fuck"); // this should never happen at this stage
                                // shit WILL go wrong
            }

            avaliable[num_i] = avaliable[8 - spot];
            avaliable[8 - spot] = num;
        }
    }

    return board;
}

void removeNumbers(usint* board, int empty) {
    // we remove the specified number of squares
    char avaliable[81];
    for (int i = 0; i < 81; i++) avaliable[i] = i;

    char num;
    int num_i;
    for (int i = 0; i < empty; i++) {
        num_i = rand() % (81 - i);
        num = avaliable[num_i];

        board[num] = 0;

        avaliable[num_i] = avaliable[80 - i];
        avaliable[80 - i] = num;
    }
}

usint* generateSudoku(int empty) {
    usint* startingBoard = getPrefilledBoard();

    usint* attempt;

    int minPos;
    usint min;
    usint curr;

    int poss;
    int num;

    int failed;

    for (int att = 0; att < 10; att++) { // we give it at most 10 attempts (I have seen it take 3 at most)
        attempt = malloc(81 * sizeof(usint));
        memcpy(attempt, startingBoard, 81 * sizeof(usint));

        failed = 0;

        
        for (int s = 0; s < 54; s++) { // we go through every uncompleted square, and collapse them 1 by 1 (the ones with leas t possibilites go first)
            minPos = 81;
            min = 0-1; // max value, everything will be smaller

            for (int pos = 0; pos < 81; pos++) { // choose one with the least options
                if ((attempt[pos] & 0b0010000000000000) == 0) continue;
                curr = attempt[pos] & 0b0001111000000000;

                if (curr < min || (curr == min && !(rand()%3))) {
                    min = curr;
                    minPos = pos;
                } 
            }

            
            // choose one of the options
            poss = min >> 9; // how many options we have
            num = rand() % poss;
            for (int i = 0; i < 9; i++) {
                if (attempt[minPos] & (1 << i)) {
                    if (!num) { // this is the number we want
                        attempt[minPos] = (i+1);
                        if (!removePossibilities(attempt, minPos)) { // a number ran out of options
                            failed = 1;
                            break;
                        }
                    } else num--;
                }
            }
            if (failed) break;
        }
        if (!failed) break;
        /* else {
            printf("fail #%d\n", att);
            printBoard(attempt);
            printf("\n");
        }*/
    }

    removeNumbers(attempt, empty);

    return attempt;
}





usint* copyBoard(usint* board) { // unused
    usint* newBoard = malloc(81 * sizeof(usint));
    memcpy(newBoard, board, 81 * sizeof(usint));
    return newBoard;
}
