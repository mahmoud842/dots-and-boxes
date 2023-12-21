#ifndef structures
#define structures

#include <stdio.h>
#include <stdlib.h>

typedef struct cell{
    // any of the sides if equal to 1 then there is a line there.
    char up;
    char down;
    char right;
    char left;

    // 0 for no one, 1 if player 1 completed it, 2 if player 2 completed it.
    // so when printing it we know which color the space should be.
    char owner;
} cell;

typedef struct state{
    // the board is intialized as 5x5 because I wanted the struct to has a constent size for reading and writing to files.
    cell grid[5][5];

    char turn; // if 0 then player 1's turn, if 1 than the player 2's turn.

    // scores are made in char bec they can't pass 255 each;
    char p1Score;
    char p2Score;
    char p1Moves;
    char p2Moves;

    int time; // number of sec passed and then we will calculate the minutes from it.
    int numberOfRemainingCells; // number of remaining incomplete cells

} state;

typedef struct options{
    // 0 for player vs player, 1 for player vs AI.
    char gameMode;

    // the actuall used size from the grid.
    char gridSize;

    // for different levels of AI.
    // hasn't defined any levels for it yet.
    char AIDifficulty;
} options;

#endif