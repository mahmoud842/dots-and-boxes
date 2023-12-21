#include "inputValidation.h"
#include <stdio.h>
#include <string.h>

#define BUFSIZE 257

// For main menu.
// 1 for start game.
// 2 for load game.
// 3 for top 10.
// 4 for exit.

// there are alot of menus in this game with different number of options this function will take the range (number of options) and then take the input and return the number the user have choosen.
// range = 1 -> number of options
int mainMenuInput(int numOfOptions){
    char buffer[BUFSIZE];
    char errorFlag = 0;
    do {
        if (errorFlag){
            printf("invalid input! try again.\n");
        }
        
        fgets(buffer, 257, stdin);
        int inputSize = strlen(buffer);   

        if (inputSize == 1){
            errorFlag = 1;
            printf("nothing? ");
        }

        else if(inputSize > 2){
            errorFlag = 1;
            while(buffer[strlen(buffer) - 1] != '\n'){
                if (!fgets(buffer, BUFSIZE, stdin)) break; // if for reaching the end of a file and there is no files here.
            }
        }

        else {
            char input = buffer[0] - '0'; // used char but i will use it as an int for less space and be the buffer[0] is a char.
            if (input > numOfOptions || input == 0)
                errorFlag = 1;
            else
                return input;
        }

    }while(errorFlag);

    printf("error happened in mainMenuInput !\n");
    return -1;
}