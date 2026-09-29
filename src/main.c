#include "sudokuGenerator.h"

#include <stdio.h>

int main() {
    for (int i = 0; i < 81; i++) {
        srand(i);
        printBoard(generateSudoku(1 * i));
        printf("\n");
    }

    return 0;
}
