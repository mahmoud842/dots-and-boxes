#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "inputValidation.h"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"



void newGameMenu(options *startGame);
void gameModeMenu(options *startGame);
void AIDiff(options *startGame);
void theOwner(state *gameState , char row , char col);

void displayMainMenu(options *startGame){

    printf("Start a new game : 1\n");
    printf("Load game : 2\n");
    printf("Display top 10 : 3\n");
    printf("exit : 4\n");

    int menuOptions = mainMenuInput(4); // the number represent which one is selected 

    switch (menuOptions) {
        case 1:
            startGame->start = 1;
            newGameMenu(startGame);
            break;
        case 2:
            startGame->loadGame = 1;
            break;
        case 3:
            startGame->displayTopTen = 1;
            break;
        case 4:
            startGame->exit = 1;
            break;
    }

}

void newGameMenu(options *startGame) {
    printf("Choose difficulty:\n");
    printf("Beginner 2*2: 1\n");;
    printf("Expert 5*5: 2\n");
    
    int difficulty = mainMenuInput(2);

    switch (difficulty) {
        case 1:
            startGame->gridSize = 2;
            break;
        case 2:
            startGame->gridSize = 5;
            break;
    }
    gameModeMenu(startGame);
}

void gameModeMenu(options *startGame){
    printf("Choose game mode:\n");
    printf("Human: 1\n");;
    printf("AI: 2\n");

    int mode = mainMenuInput(2);

        switch (mode) {
        case 1:
            startGame->gameMode = 1;
            break;
        case 2:
            AIDiff(startGame);
            startGame->gameMode = 2;
            break;
    }
}

void AIDiff(options *startGame){
    printf("Choose AI difficulty:\n");
    printf("Easy: 1\n");
    printf("Hard: 2\n");

    int aiDiff = mainMenuInput(2);

        switch (aiDiff) {
        case 1:
            startGame->AIDifficulty = 1;
            break;
        case 2:
            startGame->AIDifficulty = 2;
            break;
    }
}
// update the state to deal with up down right and left problem
// use the function makeMoveInput
// give row and col -1 
void initializeNearByCell(state *gridPosition, int row, int col , char s) { // give row and col -1
    int sizeOfGrid = gridPosition->gridSize;
    if (s == 'u') {
        gridPosition->grid[row][col].up = 1;
        if (row > 0)
        {
           gridPosition->grid[row - 1][col].down = 1; 
        }
        
    } else if (s == 'd') {
        gridPosition->grid[row][col].down = 1;
        if (row < sizeOfGrid - 1)
        {
           gridPosition->grid[row + 1][col].up = 1; 
        }
    } else if (s == 'r') {
        gridPosition->grid[row][col].right = 1;
        if (col < sizeOfGrid - 1)
        {
           gridPosition->grid[row][col + 1].left = 1 ;
        }
        
    } else if (s == 'l') {
        gridPosition->grid[row][col].left = 1;
        if (col > 0)
        {
           gridPosition->grid[row][col - 1].right = 1 ;
        }
    } else {
        printf("Try again\n");
    }

    theOwner(gridPosition,row,col);
}

void displayState(state *gameState) {
   
   
    if (gameState->turn == 0) {          // maybe it will be a mistake here cause it display the player who played last game
        printf(RED"P1 Turn\t\t"RESET);  
    } else {
        printf(BLUE"P2 Turn\t\t"RESET);
    }

    printf("Remaining Boxes : %d\n",gameState->gridSize * gameState->gridSize-gameState->numberOfRemainingCells); // we will change the name 
    
    int min = gameState->time / 60 , sec = gameState->time % 60;    // i don't think we will need hours unless the time still working after saveing

    printf("Time %d : %d\n\n",min , sec);                  

    for (int j = 1; j <= gameState->gridSize; j++) {        //number of col
        printf("  %d ", j);
    }  
    printf("\n");
    for (int i = 0; i < gameState->gridSize; i++) {       
        for (int j = 0; j < gameState->gridSize; j++) {     
           
            printf("+");
            if (gameState->grid[i][j].up == 1) {
                printf(YELLOW"---"RESET);
            } else {
                printf("   ");
            }
        }
        printf("+\n");

        for (int j = 0; j < gameState->gridSize; j++) {  
            if (gameState->grid[i][j].left == 1) {
                printf(YELLOW"|"RESET);                      // maybe modified to have a fixed color for each player   
            } else {
                printf(" ");
            }

            if (gameState->grid[i][j].owner == 1) {
                printf(RED"P 1"RESET);
            } else if (gameState->grid[i][j].owner == 2) {
                printf(BLUE"P 2"RESET);
            } else { 
                printf("   ");
            }


        }
            if (gameState->grid[i][gameState->gridSize - 1].right == 1)
            {
                printf(YELLOW"|"RESET);
            }else{
                printf(" ");
            }
            
            printf(" %d",i+1);        //number of rows
        printf("\n");
    }

    for (int j = 0; j < gameState->gridSize; j++) {
        printf("+");
        if (gameState->grid[gameState->gridSize - 1][j].down == 1) {
            printf(YELLOW"---"RESET);
        } else {
            printf("   ");
        }
    }
    printf("+\n\n");

    printf("P1 score : %d\n",gameState->p1Score);
    printf("P2 score : %d\n",gameState->p2Score);
}

void theOwner(state *gameState , char row , char col){ 
    
    if (gameState->grid[row][col].up == 1 && gameState->grid[row][col].down == 1 && gameState->grid[row][col].right == 1 &&gameState->grid[row][col].left == 1)
    {
       gameState->grid[row][col].owner = gameState->turn + 1 ;
       
       gameState->numberOfRemainingCells++; // it is confusing so we better change the name to owned boxes
       
       if (gameState->turn == 0)
       {
            gameState->p1Score += 10;      // scores start from 10
       }else{
            gameState->p2Score += 10;
       }
        
    }


}
