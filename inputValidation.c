#include "inputValidation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

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

char toLowerCase(char c){
    if (c >= 'A' && c <= 'Z'){
        return c + 32;
    }
    return c;
}

char isLetterOrSpace(char c){
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == ' '))
        return 1;
    return 0;
}

// for input in gamePlay:
// 1 (to make a move)
// 2 (undo)
// 3 (redo)
// 4 (save game)
// 5 (main menu)
// 6 (exit)
// all this 6 options can be handled by the mainMenu function 
// take the move if the player choose to make a move is left

// input for: <x> <y> <letter>
char * makeMoveInput(int gridSize){
    char buffer[BUFSIZE];
    char errorFlag = 0;
    char outOfRange = 0;
    char sideFlag = 0;
    do {
        if (errorFlag){
            printf("invalid input! try again.\n");
            if (outOfRange) {
                printf("your input should be from 1 to %d\n", gridSize);
                outOfRange = 0;
            }
            if (sideFlag){
                printf("your side should be (u for up) or (d for down) or (r for right) or (l for left)\n");
                sideFlag = 0;
            }
        }
        
        fgets(buffer, 257, stdin);
        int inputSize = strlen(buffer);   

        if (inputSize < 6){
            errorFlag = 1;
        }

        else if(inputSize > 6){
            errorFlag = 1;
            while(buffer[strlen(buffer) - 1] != '\n'){
                if (!fgets(buffer, BUFSIZE, stdin)) break; // if for reaching the end of a file and there is no files here.
            }
        }

        else {
            char x = buffer[0] - '0';
            char y = buffer[2] - '0';
            char side = buffer[4];
            if (x > gridSize || x == 0 || y > gridSize || y == 0){
                errorFlag = 1;
                outOfRange = 1;
            }
            if (!(side == 'u' || side == 'd' || side == 'r' || side == 'l')){
                errorFlag = 1;
                sideFlag = 1;
            }

            if (!sideFlag && !outOfRange){
                char * move = (char *)malloc(3 * sizeof(char));
                move[0] = x;
                move[1] = y;
                move[2] = side;
                return move;
            }
        }

    }while(errorFlag);

    printf("error happened in makeMoveInput !\n");
    return NULL;
}

// the last input needed in to take the user name as input.
char * takeUserName(){
    char buffer[BUFSIZE];
    char overSize = 0;
    char errorFlag = 0;
    char differentCharFlag = 0;
    do {
        if (overSize){
            printf("your name should be not more than %d\n", MAX_CHAR_OF_NAME - 1);
        }
        if (differentCharFlag){
            printf("your name should consist of letters or space only (note: if there is more than one space next to each other only one is taken)\n");
        }

        errorFlag = 0;
        overSize = 0;
        differentCharFlag = 0;
        
        fgets(buffer, 257, stdin);
        int inputSize = strlen(buffer);   

        if (inputSize == 1){
            errorFlag = 1;
        }

        else if(inputSize > MAX_CHAR_OF_NAME){
            overSize = 1;
            while(buffer[strlen(buffer) - 1] != '\n'){
                if (!fgets(buffer, BUFSIZE, stdin)) break; // if for reaching the end of a file and there is no files here.
            }
        }

        else {
            // validate input:
            int length = 0;
            for (;buffer[length] != '\0' && buffer[length] != '\n' ; length++){
                if (isLetterOrSpace(buffer[length]) != 1){
                    differentCharFlag = 1;
                    break;
                }
            }
            
            // not handled yet if there is more than one space next to each other
            if(!differentCharFlag){
                char * name = (char *)malloc(MAX_CHAR_OF_NAME * sizeof(char));
                char spaceFlag = 1; // if spaceFlag = 1 means I don't accept space now
                for(int i = 0, p = 0; i < MAX_CHAR_OF_NAME - 1; p++){
                    if (p < length){
                        if (buffer[p] == ' ' && !spaceFlag){
                            name[i] = buffer[p];
                            spaceFlag = 1;
                            i++;
                        }
                        else if(buffer[p] != ' '){
                            name[i] = toLowerCase(buffer[p]);
                            spaceFlag = 0;
                            i++;
                        }
                    }
                    else{
                        name[i] = ' ';
                        i++;
                    }
                }
                name[MAX_CHAR_OF_NAME - 1] = '\0';
                return name;
            }
        }

    }while(errorFlag || overSize || differentCharFlag);

    printf("error happened in takeUserName !\n");
    return NULL;
}