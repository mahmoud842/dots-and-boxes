#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "inputValidation.h"
#include "saveAndLoad.h"

void newGameMenu(options *startGame);
void gameModeMenu(options *startGame);
void AIDiff(options *startGame);
void theOwner(state *gameState , char row , char col);

void displayMainMenu(options *startGame){

    system("cls");
    printf("1) Start a new game\n");
    printf("2) Load game\n");
    printf("3) Display top 10\n");
    printf("4) exit\n");

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
    printf("1) Beginner 2*2\n");;
    printf("2) Expert 5*5\n");
    
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
    printf("1) Human\n");;
    printf("2) AI\n");

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
    printf("1) Easy\n");
    printf("2) Hard\n");

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
    system("cls");     
    for (int j = 1; j <= gameState->gridSize; j++) {        //number of col
        printf("        %d        ", j);
    }  
    printf("\n");
    for (int i = 0; i < gameState->gridSize; i++) {       
        for (int j = 0; j < gameState->gridSize; j++) {     
           
            printf("+");
            if (gameState->grid[i][j].up == 1) {
                printf(RED"---------------"RESET);
            }
            else if (gameState->grid[i][j].up == 2){
                printf(BLUE"---------------"RESET);
            } 
            else {
                printf("               ");
            }
        }
        printf("+\n");

        for (int z = 0; z < 5; z++){
            for (int j = 0; j < gameState->gridSize; j++) {  
                if (gameState->grid[i][j].left == 1) {
                    printf(RED"|"RESET);
                }
                else if (gameState->grid[i][j].left == 2){
                    printf(BLUE"|"RESET);
                }
                else {
                    printf(" ");
                }

                if (gameState->grid[i][j].owner == 1) {
                    printf("\033[41m               \033[0m");
                } else if (gameState->grid[i][j].owner == 2) {
                    printf("\033[44m               \033[0m");
                } else { 
                    printf("               ");
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
            if (z == 2)
                printf(" %d",i+1); //number of rows
            printf("\n");
        }
            
    }

    for (int j = 0; j < gameState->gridSize; j++) {
        printf("+");
        if (gameState->grid[gameState->gridSize - 1][j].down == 1) {
            printf(RED"---------------"RESET);
        } else if (gameState->grid[gameState->gridSize - 1][j].down == 2) {
            printf(BLUE"---------------"RESET);
        }
        else {
            printf("               ");
        }
    }
    printf("+\n\n");

    if (gameState->turn == 1) {
        printf(RED"P1 Turn\t\t"RESET);  
    } else {
        printf(BLUE"P2 Turn\t\t"RESET);
    }

    printf("Remaining Boxes : %d\n", gameState->numberOfRemainingCells);

    if (gameState->turn == 1){
        printf(YELLOW"P1 score: %d, P1 moves: %d\n"RESET, gameState->p1Score, gameState->p1Moves);
        printf("P2 score: %d, P2 moves: %d\n", gameState->p2Score, gameState->p2Moves);
    }
    else {
        printf("P1 score: %d, P1 moves: %d\n",gameState->p1Score, gameState->p1Moves);
        printf(YELLOW"P2 score: %d, P2 moves: %d\n"RESET,gameState->p2Score, gameState->p2Moves);
    }

    int min = gameState->time / 60 , sec = gameState->time % 60;    // i don't think we will need hours unless the time still working after saveing
    printf("Time %d : %d\n",min , sec);  

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
    printf("1) place line\n");
    printf("2) place undo\n");
    printf("3) place redo\n");
    printf("4) place save game\n");
    printf("5) place main menu\n");
    
    return mainMenuInput(5);
}

char displayAvailableFilesToLoadState(char fileNames[][14]){
    char * availableFiles = (char *)calloc(5, sizeof(char));
    checkAvailableStateFiles(fileNames, availableFiles);
    printf("Files List:\n");
    for (int i = 0; i < 5; i++){
        printf("%d) %s ", i + 1, fileNames[i]);
        if (availableFiles[i]){
            printf(RED"occupied\n"RESET);
        }
        else {
            printf(BLUE"empty\n"RESET);
        }
    }
    printf("6) return to main menu\n");

    char input = 0;
    char emptyFlag = 0;
    do {
        if (emptyFlag){
            printf(YELLOW"This file is empty\n"RESET);
            printf("choose another file: ");
            emptyFlag = 0;
        }
        else {
            printf("choose a file: ");
        }
        input = mainMenuInput(6);
        if (input == 6){
            return input;
        }
        if (!availableFiles[input - 1]){
            emptyFlag = 1;
        }

    } while (emptyFlag);

    return input;
}

char displayAvailableFilesToSaveState(char fileNames[][14], char * availableFiles){
    system("cls");
    printf("Files list:\n");
    for (int i = 0; i < 5; i++){
        printf("%d) %s ", i + 1, fileNames[i]);
        if (availableFiles[i]){
            printf(RED"occupied\n"RESET);
        }
        else {
            printf(BLUE"empty\n"RESET);
        }
    }
    printf("6) return to the game\n");

    char fileChoosen = 0;
    char anotherFileFlag = 0;
    char input1;
    do {
        if (anotherFileFlag){
            printf("choose another file\n");
            anotherFileFlag = 0;
        }
        printf("insert the number of the file you want to save to: ");
        input1 = mainMenuInput(6);
        if (input1 == 6){
            break;
        }
        input1--;

        if (availableFiles[input1]){
            printf("are you sure you want to over right this file (1 for yes, 2 for no)");
            char input2 = mainMenuInput(2);
            if (input2 == 1){
                fileChoosen = 1;
            }
            else {
                anotherFileFlag = 1;
            }
        }
        else {
            fileChoosen = 1;
        }
    } while(!fileChoosen);

    return input1;
}

// it takes struct scores only as input sorted and print the top 10 or less if ther less than 10
void displayTopTen(scores *topTenScores, int index){
    if (topTenScores->numberOfUsers <= 0) {
        printf(YELLOW"No users yet\n"RESET);
        return;
    }

  for (int i = 0; i < topTenScores->numberOfUsers - 1; i++) {
        for (int j = 0; j < topTenScores->numberOfUsers - i - 1; j++) {
            if (topTenScores->usersScores[j].score < topTenScores->usersScores[j + 1].score) {
            
                userScore temp = topTenScores->usersScores[j];
                topTenScores->usersScores[j] = topTenScores->usersScores[j + 1];
                topTenScores->usersScores[j + 1] = temp;

            }
        }
    }

    printf("Top 10 Users:\n");
    int limit;

    if (topTenScores->numberOfUsers < 10) {
        limit = topTenScores->numberOfUsers;
    } else {
        limit = 10;
    }

    for (int i = 0; i < limit; i++) {
        if (i == index)
        {
            // the color of the newest player will be yellow
            printf(YELLOW"%d- Name: %s  Score: %d\n"RESET, i + 1, topTenScores->usersScores[i].name, topTenScores->usersScores[i].score);
        }else{
        
        printf("%d- Name: %s  Score: %d\n", i + 1, topTenScores->usersScores[i].name, topTenScores->usersScores[i].score);
        }
    }

}
void croakyClose(state *s) {
    for (int i = 0; i < s->gridSize; i++) {
        for (int j = 0; j < s->gridSize; j++) {
            if (s->grid[i][j].up + s->grid[i][j].down + s->grid[i][j].right + s->grid[i][j].left == 3) {
                initializeNearByCell(s,i,j,'u') ;
                initializeNearByCell(s,i,j,'d') ;
                initializeNearByCell(s,i,j,'l') ;
                initializeNearByCell(s,i,j,'r') ;
                return;
            }
        }
    }
}
