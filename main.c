#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "inputValidation.h"
#include "display.h"
#include "saveAndLoad.h"

// compile command:
// gcc main.c structures.c inputValidation.c display.c saveAndLoad.c -o dotsAndLines
// run: ./dotsAndLines

char * startGame(state * s, options * gameOptions);

void crokyDisplayState(state * s){
    system("cls");
    for (int i = 0; i < s->gridSize; i++){
        for (int j = 0; j < s->gridSize; j++){
            printf("(%d, %d, %d, %d) ", s->grid[i][j].up, s->grid[i][j].down, s->grid[i][j].right, s->grid[i][j].left);
        }
        printf("\n");
    }
    printf("player 1 score: %d\t player 2 score: %d\n", s->p1Score, s->p2Score);
    printf("player 1 Moves: %d\t player 2 Moves: %d\n", s->p1Moves, s->p2Moves);
}

// need to place them in another place!
char leaderBoardFile[] = "highScores.bin";
char saveGameFilesNames[5][14] = {"savegame1.bin", "savegame2.bin", "savegame3.bin", "savegame4.bin", "savegame5.bin"};

int main(){
    options * gameOptions = (options *)malloc(sizeof(options));

    while(1){
        intializeOptionsWith0(gameOptions);
        displayMainMenu(gameOptions);
        if (gameOptions->start){
            //char * playerWonWithScore = startGame(NULL, gameOptions);
            char playerWonWithScore[] = {1, 5};
            if (playerWonWithScore != NULL){
                if (playerWonWithScore[0] == 1){
                    printf(RED"player %d\x1b[0m win\n", playerWonWithScore[0]);
                }
                else {
                    printf(BLUE"player %d\x1b[0m win\n", playerWonWithScore[0]);
                }
                
                scores * allScores = loadAndSortScores(leaderBoardFile);
                if (allScores == NULL){
                    printf("The leader Board file is not found or corrupted\n");
                }

                if (playerWonWithScore[0] == 1 || (playerWonWithScore[0] == 2 && gameOptions->gameMode == 1)){
                    if (allScores == NULL){
                        printf(YELLOW"a new file will be made\n"RESET);
                    }
                    printf("player %d Enter your name: ", playerWonWithScore[0]);
                    char * userName = takeUserName();
                    allScores = addUserToScores(allScores, userName, playerWonWithScore[1]);
                    free(userName);
                }

                // will be made in a seperate function this is temp
                if (allScores != NULL){
                    for (int i = 0; i < allScores->numberOfUsers; i++){
                        printf("%s %d\n", allScores->usersScores[i].name, allScores->usersScores[i].score);
                    }
                }

                printf("enter 1 to exit\n");
                char t = mainMenuInput(1);
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

char * startGame(state * s, options * gameOptions){

    if (s == NULL){
        // construct intial state
        s = constructState(gameOptions->gridSize);
        s->turn = 1; // 1 means player 1's turn, 2 means player 2 or AI turn
        // gameMode: 1 for human, 2 for AI easy, 3 for AI hard
        s->gameMode = (gameOptions->gameMode == 1) ? (gameOptions->gameMode) : (gameOptions->gameMode + gameOptions->AIDifficulty - 1);
    }

    char playerWon = 0; // 0 means no one won, 1 means player 1 won, 2 means player 2 worn (AI is considered player 2)
    char * action;

    // construct undo and redo state
    undoRedo * uR = constructUndoRedo(gameOptions->gridSize, copyState(s));

    // someFlags for error messages
    char undoFlag = 0;
    char redoFlag = 0;
    char saveGameFlag = 0;
    char elseFlag = 0;

    while(!playerWon){
        displayState(s);

        if (elseFlag){
            printf("else error\n");
            elseFlag = 0;
        }

        if (saveGameFlag){
            printf(YELLOW"game saved uccessfully\n"RESET);
            saveGameFlag = 0;
        }

        // error messages:
        if (undoFlag){
            printf(YELLOW"no undo available\n"RESET);
            undoFlag = 0;
        }
        if (redoFlag){
            printf(YELLOW"no redo available\n"RESET);
            redoFlag = 0;
        }

        char inGameMenu = displayInGameMenu();

        // for checking if AI turn:
        if (s->turn == 2 && gameOptions->gameMode == 2){
            // AI will make an action
        }
        else if (s->turn == 2 && gameOptions->gameMode == 3){
            // AI will make an action
        }
        else {

            // palce a line
            if (inGameMenu == 1){
                char invalidActionFlag = 0;
                char actionFlag = 0;

                do {
                    if (invalidActionFlag){
                        printf("invalid action!\n");
                        invalidActionFlag = 0;
                    }
                    printf("insert where you want to place your line (cell position x,y then the side u for up, d for down, l for left, r for right)\n");
                    action = makeMoveInput(gameOptions->gridSize);
                    actionFlag = applyStateAction(action, s->turn, s);
                    if (actionFlag == 0){
                        invalidActionFlag = 1;
                        free(action);
                    }
                } while(invalidActionFlag);
                

                // no condition if the actionFlag = 2 because if it is = 2 then I don't want to change player turn.
                if (actionFlag == 1){
                    s->turn = (s->turn == 1) ? 2 : 1;
                    pushStateToRedoUndo(uR, copyState(s));
                }
                else if(actionFlag == 3){
                    playerWon = s->turn;
                }
            }

            // undo
            else if (inGameMenu == 2){
                state * tmpS = getUndo(uR);
                if (tmpS != NULL){
                    freeState(s);
                    s = tmpS;
                }
                else {
                    undoFlag = 1;
                }
            }

            // redo
            else if (inGameMenu == 3){
                state * tmpS = getRedo(uR);
                if (tmpS != NULL){
                    freeState(s);
                    s = tmpS;
                }
                else {
                    redoFlag = 1;
                }
            }

            // save game
            else if (inGameMenu == 4){
                char * availableSaveGameFiles = (char *)calloc(5, sizeof(char));
                checkAvailableStateFiles(saveGameFilesNames, availableSaveGameFiles);
                char fileIndex = displayAvailableFilesForState(saveGameFilesNames, availableSaveGameFiles);
                saveStateToFile(saveGameFilesNames[fileIndex], s);
                saveGameFlag = 1;
            }

            // go to main menu
            else {
                break;
            }
        }
        
    }

    char * returnValue = NULL;
    if (playerWon == 1){
        returnValue = (char *)malloc(2 * sizeof(char));
        returnValue[0] = playerWon;
        returnValue[1] = s->p1Score;
    }
    else if (playerWon == 2){
        returnValue = (char *)malloc(2 * sizeof(char));
        returnValue[0] = playerWon;
        returnValue[1] = s->p2Score;
    }

    freeUndoRedo(uR);
    freeState(s);
    
    return returnValue;
}