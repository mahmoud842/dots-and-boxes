#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "inputValidation.h"

void newGameMenu(options *startGame);
void gameModeMenu(options *startGame);
void AIDiff(options *startGame);
void theOwner(state *gameState , char row , char col);

void displayMainMenu(options *startGame){

    system("cls");
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
    system("cls");
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
    system("cls");
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
    system("cls");
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
   
   
    if (gameState->turn == 1) {          // maybe it will be a mistake here cause it display the player who played last game
        printf(RED"P1 Turn\t\t"RESET);  
    } else {
        printf(BLUE"P2 Turn\t\t"RESET);
    }

    printf("Remaining Boxes : %d\n", gameState->numberOfRemainingCells); // we will change the name 
    
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
                printf(RED"---"RESET);
            }
            else if (gameState->grid[i][j].up == 2){
                printf(BLUE"---"RESET);
            } 
            else {
                printf("   ");
            }
        }
        printf("+\n");

        for (int j = 0; j < gameState->gridSize; j++) {  
            if (gameState->grid[i][j].left == 1) {
                printf(RED"|"RESET);                      // maybe modified to have a fixed color for each player   
            }
            else if (gameState->grid[i][j].left == 2){
                printf(BLUE"|"RESET);
            }
            else {
                printf(" ");
            }

            if (gameState->grid[i][j].owner == 1) {
                printf("\033[41m   \033[0m");
            } else if (gameState->grid[i][j].owner == 2) {
                printf("\033[44m   \033[0m");
            } else { 
                printf("   ");
            }


        }
            if (gameState->grid[i][gameState->gridSize - 1].right == 1)
            {
                printf(RED"|"RESET);
            } else if(gameState->grid[i][gameState->gridSize - 1].right == 2){
                printf(BLUE"|"RESET);
            }
            else{
                printf(" ");
            }
            
            printf(" %d",i+1);        //number of rows
        printf("\n");
    }

    for (int j = 0; j < gameState->gridSize; j++) {
        printf("+");
        if (gameState->grid[gameState->gridSize - 1][j].down == 1) {
            printf(RED"---"RESET);
        } else if (gameState->grid[gameState->gridSize - 1][j].down == 2) {
            printf(BLUE"---"RESET);
        }
        else {
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

char displayInGameMenu(){
    // 1 for place a line
    // 2 for undo
    // 3 for redo
    // 4 for save game
    // 5 for main menu

    printf("1) place line\n");
    printf("2) place undo\n");
    printf("3) place redo\n");
    printf("4) place save game\n");
    printf("5) place main menu\n");
    
    return mainMenuInput(5);
}

char displayAvailableFilesForState(char fileNames[][14], char * availableFiles){
    for (int i = 0; i < 5; i++){
        printf("%d %s ", i + 1, fileNames[i]);
        if (availableFiles[i]){
            printf(RED"occupied\n"RESET);
        }
        else {
            printf(BLUE"empty\n"RESET);
        }
    }

    char fileChoosen = 0;
    char anotherFlag = 0;
    char input1;
    do {
        if (anotherFlag){
            printf("choose another file\n");
            anotherFlag = 0;
        }
        printf("insert the number of the file you want to save to: ");
        input1 = mainMenuInput(5);
        input1--;

        if (availableFiles[input1]){
            printf("are you sure you want to over right this file (1 for yes, 2 for no)");
            char input2 = mainMenuInput(2);
            if (input2 == 1){
                fileChoosen = 1;
            }
            else {
                anotherFlag = 1;
            }
        }
        else {
            fileChoosen = 1;
        }
    } while(!fileChoosen);

    return input1;
}
