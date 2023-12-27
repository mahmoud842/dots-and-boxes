#include <stdio.h>
#include <stdlib.h>
#include "ai.h"
#include "structures.h"

// intialized with zeros using calloc
char ** constructVisited(int size){
    char ** visited = (char **)malloc(size * sizeof(char *));
    for (int i = 0; i < size; i++)
        visited[i] = (char *)calloc(size, sizeof(char));

    return visited;
}

void freeVisited(char ** visited, int size){
    for (int i = 0; i < size; i++)
        free(visited[i]);
    free(visited);
}

char fillCell(state * s, int i, int j){
    char count = 0;
    if (!s->grid[i][j].up){
        s->grid[i][j].up = s->turn;
        count++;
    }
    if (!s->grid[i][j].down){
        s->grid[i][j].down = s->turn;
        count++;
    }
    if (!s->grid[i][j].right){
        s->grid[i][j].right = s->turn;
        count++;
    }
    if (!s->grid[i][j].left){
        s->grid[i][j].left = s->turn;
        count++;
    }
    s->grid[i][j].owner = s->turn;
}

char checkSide(state * s, int i, int j, char side){
    if (s->grid[i][j].up && side == 'u')
        return 1;
    else if (s->grid[i][j].down && side == 'd')
        return 1;
    else if (s->grid[i][j].right && side == 'r')
        return 1;
    else if (s->grid[i][j].left && side == 'l')
        return 1;
    
    return 0;
}

char checkChain(state * s, char * action){
    if (checkSide(s, action[0] - 1, action[1] - 1, action[2]))
        return 2;
    char ** visited = constructVisited(s->gridSize);
    int directions[4][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    char fail = 0;
    //printf("j = %d\n", action[1] - 1);
    dfs(s, visited, action[0] - 1, action[1] - 1, -1, directions, &fail);
    //printf("past dfs\n");
    int countMoves = 0;
    int filledCells = 0;
    if (!fail){
        for (int i = 0; i < s->gridSize; i++){
            for (int j = 0; j < s->gridSize; j++){
                if (visited[i][j]){
                    countMoves += fillCell(s, i, j);
                    filledCells++;
                }
            }
        }
        s->numberOfRemainingCells -= filledCells;
        if (s->turn == 1){
            s->p1Moves += filledCells - 1;
            s->p1Score += filledCells;
        }
        else {
            s->p2Moves += filledCells - 1;
            s->p2Score += filledCells;
        }
        freeVisited(visited, s->gridSize);
        return 1;
    }
    freeVisited(visited, s->gridSize);
    return 0;
}

// checking if (the border is missing) is missing
// came from 0->right, 1->left, 2->up, 3->down;
// movements = [(0, 1), (0, -1), (1, 0), (-1, 0)]
void dfs(state * s, char ** visited, int i, int j, int cameFrom, int directions[4][2], char * fail){
    //printf("dfs call\n");
    //printf("i = %d, j = %d\n", i, j);
    if (*fail){
        return;
    }
    visited[i][j] = 1;

    int newI;
    int newJ;
    for (int dir = 0; dir < 4; dir++){
        if (dir == cameFrom){
            //printf("inside first condition\n");
            continue;
        }
        else if (dir == 0 && s->grid[i][j].right){
            //printf("inside dir = 0 condition\n");
            continue;
        }
        else if (dir == 1 && s->grid[i][j].left){
            //printf("inside dir = 1 condition\n");
            continue;

        }
        else if (dir == 2 && s->grid[i][j].down){
            //printf("inside dir = 2 condition\n");
            continue;

        }
        else if (dir == 3 && s->grid[i][j].up){
            //printf("inside dir = 3 condition\n");
            continue;
        }
        
        newI = i + directions[dir][0];
        newJ = j + directions[dir][1];
        if ((newI >= 0 && newI < s->gridSize) && (newJ >= 0 && newJ < s->gridSize) && !visited[newI][newJ]){
            int newCameFrom;
            if (dir == 0)
                newCameFrom = 1; 
            else if (dir == 1)
                newCameFrom = 0;
            else if (dir == 2)
                newCameFrom = 3;
            else newCameFrom = 2;
            //printf("newI = %d, newJ = %d, newCameFrom = %d\n", newI, newJ, newCameFrom);
            dfs(s, visited, newI, newJ, newCameFrom, directions, fail);
        }
        else {
            *fail = 1;
            return;
        }
    }
}