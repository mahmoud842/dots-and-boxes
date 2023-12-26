#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "inputValidation.h"
#include "display.h"
#include "saveAndLoad.h"

// compile command:
// gcc main.c structures.c inputValidation.c display.c saveAndLoad.c -o dotsAndLines
// run: ./dotsAndLines

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

int main(){
    char leaderBoardFile[] = "highScores.bin";
    char saveGameFilesNames[5][14] = {"savegame1.bin", "savegame2.bin", "savegame3.bin", "savegame4.bin", "savegame5.bin"};
    options * gameOptions = (options *)malloc(sizeof(options));

    while(1){
        intializeOptionsWith0(gameOptions);
        displayMainMenu(gameOptions);
        if (gameOptions->start){
            char playerTurn = 1; // 1 means player 1's turn, 2 means player 2's turn, 0 means AI turn
            char playerWon = 0; // 0 means no one won, 1 means player 1 won, 2 means player 2 worn (AI is considered player 2)
            char * action;
            // construct intial state
            state * s = constructState(gameOptions->gridSize);

            // construct undo and redo state
            s->turn = playerTurn;
            undoRedo * uR = constructUndoRedo(gameOptions->gridSize, copyState(s));

            // someFlags for error messages
            char undoFlag = 0;
            char redoFlag = 0;
            char elseFlag = 0;
            char saveGameFlag = 0;

            while(!playerWon){
                displayState(s);

                if (elseFlag){
                    printf("else error\n");
                    elseFlag = 0;
                }

                if (saveGameFlag){
                    printf("save error\n");
                    saveGameFlag = 1;
                }

                // error messages:
                if (undoFlag){
                    printf("no undo available\n");
                    undoFlag = 0;
                }
                if (redoFlag){
                    printf("no redo available\n");
                    redoFlag = 0;
                }

                // gameMode: 1 for human, 2 for AI easy, 3 for AI hard
                s->gameMode = (gameOptions->gameMode == 1) ? (gameOptions->gameMode) : (gameOptions->gameMode + gameOptions->AIDifficulty - 1);
                // here I should as about input first (savegame, undo ,redo ,..)
                // printf("player %d %d turn\n", playerTurn, s->turn);
                char inGameMenu = displayInGameMenu();
                

                // palce a line
                if (inGameMenu == 1){
                    char invalidActionFlag = 0;
                    char actionFlag = 0;

                    // printf("inside place line condition\n");

                    // if playerTurn = 1 or 2 then human plays
                    if (playerTurn){
                        do {
                            if (invalidActionFlag){
                                printf("invalid action!\n");
                                invalidActionFlag = 0;
                            }
                            action = makeMoveInput(gameOptions->gridSize);
                            actionFlag = applyStateAction(action, playerTurn, s);
                            if (actionFlag == 0){
                                invalidActionFlag = 1;
                                free(action);
                            }
                        } while(invalidActionFlag);

                        

                    }

                    // AI turn
                    else {
                        // action = AIAction();
                        // actionFlag = applyStateAction(action, playerTurn, s)
                        printf("AI not available now\n");
                    }

                    // no condition if the actionFlag = 2 because if it is = 2 then I don't want to change player turn.
                    if (actionFlag == 1){
                        if (s->gameMode == 1){
                            playerTurn = (playerTurn == 1) ? 2 : 1;
                        }
                        else {
                            playerTurn = (playerTurn == 1) ? 0 : 1;
                        }
                        s->turn = playerTurn;
                        pushStateToRedoUndo(uR, copyState(s));
                    }
                    else if(actionFlag == 3){
                        printf("player %d win\n", playerTurn);
                        playerWon = playerTurn;
                    }
                }

                // undo
                else if (inGameMenu == 2){
                    state * tmpS = getUndo(uR);
                    if (tmpS != NULL){
                        freeState(s);
                        s = tmpS;
                        playerTurn = s->turn;
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
                        playerTurn = s->turn;
                    }
                    else {
                        redoFlag = 1;
                    }
                }

                // save game
                else if (inGameMenu == 4){
                    saveGameFlag = 1;
                }

                // go to main menu
                else {
                    elseFlag = 1;
                }

                
            }
            freeUndoRedo(uR);
            crokyDisplayState(s);
            freeState(s);
            printf("here i should take your name as input");
            
            // just to stop it for now (will be changed)
            char ttt;
            scanf("%c", ttt);
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