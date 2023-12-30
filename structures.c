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

char checkCellFilled(cell * c){
    if (c->up && c->down && c->right && c->left){
        return 1;
    }
    return 0;
}

// state:
// size of grid = 5 if you want 5x5, size = 2 if you want 2x2 and so on.
state * constructState(int sizeOfGrid){
    state * s = calloc(1, sizeof(state));
    
    s->grid = (cell **)malloc(sizeOfGrid * sizeof(cell *));
    for (int i = 0; i < sizeOfGrid; i++){
        // I used calloc bec it intializes with zero and no need for me to intialize it
        s->grid[i] = (cell *)calloc(sizeOfGrid, sizeof(cell));
    }

    s->gridSize = sizeOfGrid;
    s->numberOfRemainingCells = sizeOfGrid * sizeOfGrid;
    return s;
}

void freeState(state * s){
    for (int i = 0; i < s->gridSize; i++){
        free(s->grid[i]);
    }
    free(s->grid);
    free(s);
}

// return zero if invalid move, 1 if valid, 2 if the player completed a box and should play again, 3 if the game ends
char applyStateAction(char * action, char playerTurn, state * s){
    int i = action[0] - 1;
    int j = action[1] - 1;
    int numberOfFilledBoxes = 0;

    if (action[2] == 'u'){
        if (s->grid[i][j].up != 0){
            return 0;
        }

        s->grid[i][j].up = playerTurn;

        if (i > 0){
            s->grid[i-1][j].down = playerTurn;
            if (checkCellFilled(&(s->grid[i - 1][j]))){
                numberOfFilledBoxes++;
                s->grid[i - 1][j].owner = playerTurn;
            }
        } 
            
    }
    else if (action[2] == 'd'){
        if (s->grid[i][j].down != 0){
            return 0;
        }

        s->grid[i][j].down = playerTurn;

        if (i < s->gridSize - 1){
            s->grid[i + 1][j].up = playerTurn;
            if (checkCellFilled(&(s->grid[i + 1][j]))){
                numberOfFilledBoxes++;
                s->grid[i + 1][j].owner = playerTurn;
            }
        } 
    }
    else if (action[2] == 'r'){
        if (s->grid[i][j].right != 0){
            return 0;
        }

        s->grid[i][j].right = playerTurn;

        if (j < s->gridSize - 1){
            s->grid[i][j + 1].left = playerTurn;
            if (checkCellFilled(&(s->grid[i][j + 1]))){
                numberOfFilledBoxes++;
                s->grid[i][j + 1].owner = playerTurn;
            }
        } 
    }
    else {
        if (s->grid[i][j].left != 0){
            return 0;
        }

        s->grid[i][j].left = playerTurn;

        if (j > 0){
            s->grid[i][j - 1].right = playerTurn;
            if (checkCellFilled(&(s->grid[i][j - 1]))){
                numberOfFilledBoxes++;
                s->grid[i][j - 1].owner = playerTurn;
            }
        } 
    }

    if (checkCellFilled(&(s->grid[i][j]))){
        numberOfFilledBoxes++;
        s->grid[i][j].owner = playerTurn;
    }

    if (s->turn == 1){
        s->p1Moves++;
        if (numberOfFilledBoxes){
            s->p1Score += numberOfFilledBoxes;
        }
    }
    else {
        s->p2Moves++;
        if (numberOfFilledBoxes){
            s->p2Score += numberOfFilledBoxes;
        }
    }

    s->numberOfRemainingCells -= numberOfFilledBoxes;
    if (s->numberOfRemainingCells == 0){
        return 3;
    }

    if (numberOfFilledBoxes != 0){
        return 2;
    }

    return 1;
}

// this function returns a copy of the state
state * copyState(state * s){
    state * newState = constructState(s->gridSize);
    newState->gameMode = s->gameMode;
    newState->gridSize = s->gridSize;
    newState->p1Moves = s->p1Moves;
    newState->p1Score = s->p1Score;
    newState->p2Moves = s->p2Moves;
    newState->p2Score = s->p2Score;
    newState->numberOfRemainingCells = s->numberOfRemainingCells;
    newState->time = s->time;
    newState->turn = s->turn;
    for (int i = 0; i < s->gridSize; i++){
        for (int j = 0; j < s->gridSize; j++){
            newState->grid[i][j].up = s->grid[i][j].up;
            newState->grid[i][j].down = s->grid[i][j].down;
            newState->grid[i][j].right = s->grid[i][j].right;
            newState->grid[i][j].left = s->grid[i][j].left;
            newState->grid[i][j].owner = s->grid[i][j].owner;
        }
    }
    return newState;
}

void freeScores(scores * s){
    for (int i = 0; i < s->numberOfUsers; i++){
        free(s->usersScores[i].name);
    }
    free(s->usersScores);
    free(s);
}

// num instead of number because numberOfUsers is defined in the scores structure.
scores * constructScores(int numOfUsers){
    scores * scoresPtr = (scores *)malloc(sizeof(scores));
    scoresPtr->numberOfUsers = numOfUsers;
    scoresPtr->usersScores = (userScore *)malloc(numOfUsers * sizeof(userScore));
    for (int i = 0; i < numOfUsers; i++){
        scoresPtr->usersScores[i].name = (char *)malloc(MAX_CHAR_OF_NAME * sizeof(char));
    }
    return scoresPtr;
}

void intializeOptionsWith0(options * gameOptions){
    gameOptions->start = 0;
    gameOptions->loadGame = 0;
    gameOptions->displayTopTen = 0;
    gameOptions->exit = 0;
    gameOptions->gameMode = 0;
    gameOptions->AIDifficulty = 0;
    gameOptions->gridSize = 0;
}

// undoRedo:

// each state size is 140 byte
// if grid is 2x2 then max of 12 states will be made worst case memory = 12 * 140 = 1680 bytes (1.6 KB)
// if grid is 5x5 then max of 60 states will be made worst case memory = 60 * 140 = 8400 bytes (8.2 KB)
// this calculation without pointer sizes and others

// 1) index = lastIndex : means that there is no redo and the current displayed state isn't in the stateArr
// 2) index = lastIndex - 1: means that the displayed state is the lastIndex state and there is no redo
// 3) index = -1 : means that there is no undo
// 4) if index = i and user want redo then the redo is at index + 2

undoRedo * constructUndoRedo(int gridSize, state * intialState){
    undoRedo * u = (undoRedo *)malloc(sizeof(undoRedo));
    u->maxCapacity = (gridSize == 2) ? 12 : 60;
    u->index = 0;
    u->lastIndex = 0;
    u->stateArr = (state **)malloc(u->maxCapacity * sizeof(state *));
    u->stateArr[0] = intialState;
    return u;
}

void freeUndoRedo(undoRedo * u){
    for (int i = 0; i <= u->lastIndex; i++){
        freeState(u->stateArr[i]);
    }
    free(u->stateArr);
    free(u);
}

void deleteRedo(undoRedo * u){
    for (int i = u->index + 1; i <= u->lastIndex; i++){
        freeState(u->stateArr[i]);
    }
    u->lastIndex = u->index;
}

// also deletes all redo
void pushStateToRedoUndo(undoRedo * u, state * s){
    deleteRedo(u);
    u->stateArr[u->index + 1] = s;
    u->index++;
    u->lastIndex++;
}

state * getUndo(undoRedo * u){
    if (u->index == 0){
        return NULL;
    }

    state * s = u->stateArr[u->index - 1];
    u->index--;
    
    return copyState(s);
}

state * getRedo(undoRedo * u){
    if (u->lastIndex == u->index){
        return NULL;
    }

    state * s = u->stateArr[u->index + 1];
    u->index++;
    return copyState(s);
}

char compareStr(char * name1, char * name2){
    for (int i = 0; i < MAX_CHAR_OF_NAME; i++){
        if (name1[i] != name2[i])
            return 0;
    }
    return 1;
}

char userInScores(scores * s, char * userName, int score, int * index){
    if (s == NULL){
        return 0;
    }

    for (int i = 0; i < s->numberOfUsers; i++){
        if (compareStr(s->usersScores[i].name, userName)){
            if (s->usersScores[i].score < score){
                s->usersScores[i].score = score;
            }
            printf("old index = %d\n", i);
            *index = putInPosition(s, i);
            printf("new index = %d\n", *index);
            return 1;
        }
    }
    return 0;
}

void swapUserScores(userScore * ptr1, userScore * ptr2){
    userScore * tmp = (userScore *)malloc(sizeof(userScore));
    tmp->name = ptr1->name;
    tmp->score = ptr1->score;

    ptr1->name = ptr2->name;
    ptr1->score = ptr2->score;

    ptr2->name = tmp->name;
    ptr2->score = tmp->score;

    free(tmp);
}

// this function load and sort the scores from the file and if the file doesn't exist or corrupted or empty returns NULL.
void sortScores(scores * s){
    for (int i = 0; i < s->numberOfUsers - 1; i++){
        for (int j = 0; j < s->numberOfUsers - i - 1; j++){
            if (s->usersScores[j].score < s->usersScores[j + 1].score){
                swapUserScores(&(s->usersScores[j]), &(s->usersScores[j + 1]));
            }
        }
    }
}

int putInPosition(scores * s, int index){
    for (int i = index; i > 0; i--){
        if (s->usersScores[i].score > s->usersScores[i - 1].score){
            swapUserScores(&(s->usersScores[i]), &(s->usersScores[i - 1]));
        }
        else {
            printf("index inside putIn = %d\n", i);
            return i;
        }
    }
    return 0;
}