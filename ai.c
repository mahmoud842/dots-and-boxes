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

void intializeVisitedWithZeros(char ** visited, int size){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            visited[i][j] = 0;
        }
    }
}

char fillCell(state * s, int i, int j){
    char count = 0;
    if (!s->grid[i][j].up){
        s->grid[i][j].up = s->turn;
        count++;
        if (i > 0){
            s->grid[i-1][j].down = s->turn;
        }
    }
    if (!s->grid[i][j].down){
        s->grid[i][j].down = s->turn;
        count++;
        if (i < s->gridSize - 1){
            s->grid[i + 1][j].up = s->turn;
        }
    }
    if (!s->grid[i][j].right){
        s->grid[i][j].right = s->turn;
        count++;
        if (j < s->gridSize - 1){
            s->grid[i][j + 1].left = s->turn;
        }
    }
    if (!s->grid[i][j].left){
        s->grid[i][j].left = s->turn;
        count++;
        if (j > 0){
            s->grid[i][j - 1].right = s->turn;
        }
    }
    s->grid[i][j].owner = s->turn;
    return count;
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
    dfs(s, visited, action[0] - 1, action[1] - 1, -1, directions, &fail);
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

// came from 0->right, 1->left, 2->up, 3->down;
// movements = [(0, 1), (0, -1), (1, 0), (-1, 0)]
void dfs(state * s, char ** visited, int i, int j, int cameFrom, int directions[4][2], char * fail){
    if (*fail){
        return;
    }
    visited[i][j] = 1;

    int newI;
    int newJ;
    for (int dir = 0; dir < 4; dir++){
        if (dir == cameFrom){
            continue;
        }
        else if (dir == 0 && s->grid[i][j].right){
            continue;
        }
        else if (dir == 1 && s->grid[i][j].left){
            continue;

        }
        else if (dir == 2 && s->grid[i][j].down){
            continue;

        }
        else if (dir == 3 && s->grid[i][j].up){
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
            dfs(s, visited, newI, newJ, newCameFrom, directions, fail);
        }
        else {
            *fail = 1;
            return;
        }
    }
}

int ** constructCellsScores(int size){
    int ** cellsScores = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
        cellsScores[i] = (int *)calloc(size, sizeof(int));

    return cellsScores;
}

void freeCellsScores(int ** cellsScores, int size){
    for (int i = 0; i < size; i++)
        free(cellsScores[i]);
    free(cellsScores);
}

int countCellSides(cell c){
    int count = 0;
    if (c.up)
        count++;
    if (c.down)
        count++;
    if (c.right)
        count++;
    if (c.left)
        count++;
    return count;
}

// came from 0->right, 1->left, 2->up, 3->down;
// movements = [(0, 1), (0, -1), (1, 0), (-1, 0)]
void AIDFS(state * s, char ** visited, int i, int j, int directions[4][2], int * countChain){
    visited[i][j] = 1;
    if (countCellSides(s->grid[i][j]) != 3){
        return;
    }
    fillCell(s, i, j);
    *countChain += 1;
    // printf("reached\n");

    int newI;
    int newJ;
    for (int dir = 0; dir < 4; dir++){        
        newI = i + directions[dir][0];
        newJ = j + directions[dir][1];
        if ((newI >= 0 && newI < s->gridSize) && (newJ >= 0 && newJ < s->gridSize) && !visited[newI][newJ]){
            AIDFS(s, visited, newI, newJ, directions, countChain);
        }
    }
}

// this part could be better by checking the intersect cell and the number of sides already in it
// and choose the side that intersect with the cell with least sides
char * generateActionOfAI(state * s, int i, int j, int score){
    char * action = (char *)malloc(3 * sizeof(char));
    action[0] = i + 1;
    action[1] = j + 1;
    int directions[4][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    int sideScores[4] = {100, 100, 100, 100};

    for (int dir = 0; dir < 4; dir++){
        if (dir == 0 && s->grid[i][j].right){
            continue;
        }
        else if (dir == 1 && s->grid[i][j].left){
            continue;

        }
        else if (dir == 2 && s->grid[i][j].down){
            continue;

        }
        else if (dir == 3 && s->grid[i][j].up){
            continue;
        }
        int newI = i + directions[dir][0];
        int newJ = j + directions[dir][1];
        if ((newI >= 0 && newI < s->gridSize) && (newJ >= 0 && newJ < s->gridSize)){
            sideScores[dir] = countCellSides(s->grid[newI][newJ]);
        }
        else 
            sideScores[dir] = 0;
    }

    // get least value:
    int leastScore = sideScores[0];
    int leastIndex = 0;
    for (int i = 0; i < 4; i++){
        if (leastScore > sideScores[i]){
            leastScore = sideScores[i];
            leastIndex = i;
        }
    }

    if (leastIndex == 0){
        action[2] = 'r';
    }
    else if (leastIndex == 1){
        action[2] = 'l';
    }
    else if (leastIndex == 2){
        action[2] = 'd';
    }
    else {
        action[2] = 'u';
    }

    return action;
}

// each cell is given a score and the cell with the highest score will be choosen
// 1- a chain is given a score to only one of its cell = number of cells in the chain * 4
// note: that AI chains aren't just like the chain algorithm of auto filling
//       its a more open chain just checking if we filled this what will happen 
//       to the next and then keep tracking until we can't fill anymore and this means the end of the chain
// 2- a box with 3 sides will be given 4
// 3- a box with 0 sides will be given 3
// 4- a box with 1 side will be given 2
// 5- a box with 2 sides will be given 1
char * hardAIAction(state * s){
    char ** visited = constructVisited(s->gridSize);
    int ** cellsScores = constructCellsScores(s->gridSize);
    int directions[4][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    char fail = 0;
    state * newTmpState = NULL;
    int countScore = 0;
    for (int i = 0; i < s->gridSize; i++){
        for (int j = 0; j < s->gridSize; j++){
            newTmpState = copyState(s);
            intializeVisitedWithZeros(visited, s->gridSize);
            countScore = 0;
            AIDFS(newTmpState, visited, i, j, directions, &countScore);
            // printf("%d ", countScore);
            if (countScore == 0){
                int numberOfSides = countCellSides(s->grid[i][j]);
                if (numberOfSides == 4)
                    cellsScores[i][j] = -1;
                else
                    cellsScores[i][j] = 3 - numberOfSides;
            }
            else {
                cellsScores[i][j] = countScore * 4;
            }
            freeState(newTmpState);
        }
        // printf("\n");
    }

    // test: print cells scores
    // for (int i = 0; i < s->gridSize; i++){
    //     for (int j = 0; j < s->gridSize; j++){
    //         printf("%d ", cellsScores[i][j]);
    //     }
    //     printf("\n");
    // }


    // select the highest score:
    int maxScore = cellsScores[0][0];
    int maxI = 0;
    int maxJ = 0;

    for (int i = 0; i < s->gridSize; i++){
        for (int j = 0; j < s->gridSize; j++){
            if (cellsScores[i][j] > maxScore){
                maxScore = cellsScores[i][j];
                maxI = i;
                maxJ = j;
            }
        }
    } 

    char * AIAction;
    if (maxScore == -1){
        AIAction = NULL;
    }
    else {
        AIAction = generateActionOfAI(s, maxI, maxJ, maxScore);
    }

    freeVisited(visited, s->gridSize);
    freeCellsScores(cellsScores, s->gridSize);
    return AIAction;
}