#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "inputValidation.h"
#include "display.h"
#include "saveAndLoad.h"

// compile command:
// gcc main.c structures.c inputValidation.c display.c saveAndLoad.c -o dotsAndLines
// run: ./dotsAndLines

int main(){
    char leaderBoardFile[] = "highScores.bin";
    char saveGameFilesNames[5][14] = {"savegame1.bin", "savegame2.bin", "savegame3.bin", "savegame4.bin", "savegame5.bin"};
    options * gameOptions = (options *)malloc(sizeof(options));

    while(1){
        intializeOptionsWith0(gameOptions);
        displayMainMenu(gameOptions);
        if (gameOptions->start){
            if (gameOptions->gameMode == 1){

            }
            else if (gameOptions->gameMode == 2){

            }
            else {
                printf("error 1\n");
                return 1;
            }
        }
        else if (gameOptions->loadGame){

        }
        else if(gameOptions->displayTopTen){

        }
        else if(gameOptions->exit){
            printf("Thank You for playing the game\n");
            break;
        }
        else {
            // this condition was made to for debugging unlikly to happen in the game.
            printf("error happened in the first gamee options input\n");
            break;
        }
    }

    return 0;
}