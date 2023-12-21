#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "inputValidation.h"
#include "display.h"
#include "saveAndLoad.h"

// compile command:
// gcc main.c structures.c inputValidation.c display.c saveAndLoad.c -o dotsAndLines
// run: ./dotsAndLines

int main(){
    printf("hello\n");
    state * s = constructState(5);
    for (int i = 0; i < s->gridSize; i++){
        for (int j = 0; j < s->gridSize; j++){
            printf("(%d, %d, %d, %d, %d) ", s->grid[i][j].up, s->grid[i][j].down, s->grid[i][j].right, s->grid[i][j].left, s->grid[i][j].owner);
        }
        printf("\n");
    }
    freeState(s);
    return 0;
}