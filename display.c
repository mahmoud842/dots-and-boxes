#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "inputValidation.h"



void newGameMenu(options *startGame);
void gameModeMenu(options *startGame);
void AIDiff(options *startGame);

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
