#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

// constructors, intialize and free for each structure:
// cell:
cell * constructCell(){
    cell * c = (cell *)calloc(1, sizeof(cell));
}

cell * intialize(cell * c, char u, char d, char r, char l, char own){
    if (c == NULL)
        c = constructCell();

    c->up = u;
    c->down = d;
    c->right = r;
    c->left = l;
    c->owner = own;
}

void freeCell(cell * c){
    free(c);
}

// state:
// size of grid = 5 if you want 5x5, size = 2 if you want 2x2 and so on.
state * constructState(int sizeOfGrid){
    state * s = calloc(1, sizeof(state));
    
    s->grid = (cell **)malloc(sizeOfGrid * sizeof(cell *));
    for (int i = 0; i < 5; i++){
        // I used calloc bec it intializes with zero and no need for me to intialize it
        s->grid[i] = (cell *)calloc(sizeOfGrid, sizeof(cell));
    }

    s->gridSize = sizeOfGrid;
}

void freeState(state * s){
    for (int i = 0; i < s->gridSize; i++){
        free(s->grid[i]);
    }
    free(s->grid);
    free(s);
}