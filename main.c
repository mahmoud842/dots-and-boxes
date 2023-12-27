#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "inputValidation.h"
#include "display.h"
#include "saveAndLoad.h"
#include "ai.h"

// compile command:
// gcc main.c structures.c inputValidation.c display.c saveAndLoad.c ai.c -o dotsAndLines
// run: ./dotsAndLines

int * startGame(state * s, options * gameOptions);
scores * leaderBoard(char playerWon, int scoreOfWinner, int * index);
void displayWonAndLeaderBoard(int * playerWonWithScore, options * gameOptions);
void startGameMask(state * s, options * gameOptions);

// need to place them in another place!
char leaderBoardFile[] = "highScores.bin";
char saveGameFilesNames[5][14] = {"savegame1.bin", "savegame2.bin", "savegame3.bin", "savegame4.bin", "savegame5.bin"};

int main(){
    options * gameOptions = (options *)malloc(sizeof(options));

    while(1){
        intializeOptionsWith0(gameOptions);
        displayMainMenu(gameOptions);
        if (gameOptions->start){
            startGameMask(NULL, gameOptions);
        }
        else if (gameOptions->loadGame){
            state * s = NULL;
            char fileIsBad = 0;
            char input = 0;
            do {
                if (fileIsBad){
                    printf("This file is corrupted please choose another file: \n");
                }
                input = displayAvailableFilesToLoadState(saveGameFilesNames);
                if (input == 6){
                    break;
                }
                s = loadStateFromFile(saveGameFilesNames[input - 1]);
                if (s == NULL){
                    fileIsBad = 1;
                }
            } while (fileIsBad);
            if (input != 6){
                gameOptions->gameMode = s->gameMode;
                gameOptions->gridSize = s->gridSize;
                startGameMask(s, gameOptions);
            }

        }
        else if(gameOptions->displayTopTen){
            scores * allScores = loadAndSortScores(leaderBoardFile);
            if (allScores == NULL){
                printf(YELLOW"The leader Board file is not found or corrupted or empty\n"RESET);
            }
            else {
                displayTopTen(allScores, -1);
            }
            printf("enter 1 to return to main menu\n");
            char t = mainMenuInput(1);
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

void displayWonAndLeaderBoard(int * playerWonWithScore, options * gameOptions){
    if (playerWonWithScore != NULL){
        if (playerWonWithScore[0] == 1){
            printf(RED"player %d\x1b[0m win\n", playerWonWithScore[0]);
        }
        else {
            if (gameOptions->gameMode == 2){
                printf(BLUE"AI\x1b[0m win\n");
            }
            else {
                printf(BLUE"player %d\x1b[0m win\n", playerWonWithScore[0]);
            }
        }
        
        // handle the leader board part
        if (playerWonWithScore[0] == 1 || (playerWonWithScore[0] == 2 && gameOptions->gameMode == 1)){
            int index = -1;
            scores * allScores = leaderBoard(playerWonWithScore[0], playerWonWithScore[1], &index);
            system("cls");
            displayTopTen(allScores, index);
            saveScoresToFile(leaderBoardFile, allScores);
        }
        else {
            scores * allScores = loadAndSortScores(leaderBoardFile);
            if (allScores == NULL){
                printf("The leader Board file is not found or corrupted\n");
                printf(YELLOW"a new file will be made\n"RESET);
            }
            else {
                displayTopTen(allScores, -1);
            }
        }
        printf("enter 1 to got to main menu\n");
        char t = mainMenuInput(1);
    }
}

scores * leaderBoard(char playerWon, int scoreOfWinner, int * index){
    scores * allScores = loadAndSortScores(leaderBoardFile);

    if (allScores == NULL){
        printf("The leader Board file is not found or corrupted\n");
        printf(YELLOW"a new file will be made\n"RESET);
    }

    printf("player %d Enter your name: ", playerWon);
    char * userName = takeUserName();

    if (!userInScores(allScores, userName, scoreOfWinner, index)){
        allScores = addUserToScores(allScores, userName, scoreOfWinner, index);
    }

    free(userName);
    return allScores;
}

// some lines to handle tie i didn't want to change in startGame so i made this function to do the logic then call startGame.
void startGameMask(state * s, options * gameOptions){
    int * playerWonWithScore = startGame(s, gameOptions);
    if (playerWonWithScore == NULL) return;
    if (playerWonWithScore[0] == 0){
        printf(YELLOW"Tie\n"RESET);
        scores * allScores = loadAndSortScores(leaderBoardFile);
        if (allScores == NULL){
            printf("The leader Board file is not found or corrupted or empty\n");
        }
        else {
            displayTopTen(allScores, -1);
        }
        printf("enter 1 to got to main menu\n");
        char t = mainMenuInput(1);
    }
    else if (playerWonWithScore != NULL){
        displayWonAndLeaderBoard(playerWonWithScore, gameOptions);
    }
}

int * startGame(state * s, options * gameOptions){

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
            printf(YELLOW"game saved successfully\n"RESET);
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


        // Easy AI turn
        if (s->turn == 2 && s->gameMode == 2){
      
            char * action = beginnerAi(s);
            int actionFlag = applyStateAction(action, s->turn, s);
            if (actionFlag == 1){
                s->turn = (s->turn == 1) ? 2 : 1;
            }
            else if(actionFlag == 3){
                displayState(s);
                if (s->p1Score > s->p2Score){
                    playerWon = 1;
                }
                else if (s->p1Score < s->p2Score){
                    playerWon = 2;
                }
                else {
                    playerWon = 3;
                }
            }

        }
        // hard AI turn
        else if (s->turn == 2 && s->gameMode == 3){
            char * action = hardAIAction(s);
            int actionFlag = applyStateAction(action, s->turn, s);
            if (actionFlag == 1){
                s->turn = (s->turn == 1) ? 2 : 1;
            }
            else if(actionFlag == 3){
                displayState(s);
                if (s->p1Score > s->p2Score){
                    playerWon = 1;
                }
                else if (s->p1Score < s->p2Score){
                    playerWon = 2;
                }
                else {
                    playerWon = 3;
                }
            }
        }
        else {
            char inGameMenu = displayInGameMenu();

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
                    char chainFlag = checkChain(s, action);
                    if (chainFlag == 1){
                        if (s->numberOfRemainingCells == 0)
                            actionFlag = 3;
                        else 
                            actionFlag = 2;
                    }
                    else if (chainFlag == 0){
                        actionFlag = applyStateAction(action, s->turn, s);
                    }
                    if (actionFlag == 0){
                        invalidActionFlag = 1;
                        free(action);
                    }
                } while(invalidActionFlag);
                
                if (actionFlag == 1){
                    s->turn = (s->turn == 1) ? 2 : 1;
                    pushStateToRedoUndo(uR, copyState(s));
                }
                else if (actionFlag == 2){
                    pushStateToRedoUndo(uR, copyState(s));
                }
                else if(actionFlag == 3){
                    displayState(s);
                    if (s->p1Score > s->p2Score){
                        playerWon = 1;
                    }
                    else if (s->p1Score < s->p2Score){
                        playerWon = 2;
                    }
                    else {
                        playerWon = 3;
                    }
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
                char fileIndex = displayAvailableFilesToSaveState(saveGameFilesNames, availableSaveGameFiles);
                if (fileIndex != 6){
                    saveStateToFile(saveGameFilesNames[fileIndex], s);
                    saveGameFlag = 1;
                }
            }

            // go to main menu
            else {
                break;
            }
        }
        
    }

    int * returnValue = NULL;
    if (playerWon == 1){
        returnValue = (int *)malloc(2 * sizeof(int));
        returnValue[0] = playerWon;
        returnValue[1] = s->p1Score;
    }
    else if (playerWon == 2){
        returnValue = (int *)malloc(2 * sizeof(int));
        returnValue[0] = playerWon;
        returnValue[1] = s->p2Score;
    }
    else if (playerWon == 3){
        returnValue = (int *)malloc(2 * sizeof(int));
        returnValue[0] = 0;
        returnValue[1] = -1;
    }

    freeUndoRedo(uR);
    freeState(s);
    
    return returnValue;
}