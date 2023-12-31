#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"
#include "inputValidation.h"
#include "display.h"
#include "saveAndLoad.h"
#include "ai.h"

// compile command:
// gcc main.c structures.c inputValidation.c display.c saveAndLoad.c ai.c -o dotsAndLines
// run: ./dotsAndLines

void gameLoop(state * _state, options * gameOptions);
char applyActionFlag(state * _state, undoRedo * _undoRedoStruct, int actionFlag);
int makeAnAction(state * _state, options * gameOptions);
char validateUndoRedo(state ** _state, state ** undoRedoState);
void continueInput();
void displayWinner(int winner, char gameMode);
void leaderBoard(char winner, char * playerName, int playerScore);
state * loadGame(options * gameOptions);

char leaderBoardFile[] = "highScores.bin";
char saveGameFilesNames[5][14] = {"savegame1.bin", "savegame2.bin", "savegame3.bin", "savegame4.bin", "savegame5.bin"};

int main(){
    options * gameOptions = (options *)malloc(sizeof(options));

    while(1){
        intializeOptionsWith0(gameOptions);
        displayMainMenu(gameOptions);
        if (gameOptions->start){
            gameLoop(NULL, gameOptions);
        }
        else if (gameOptions->loadGame){
            state * loadedState = loadGame(gameOptions);
            if (loadedState != NULL)
                gameLoop(loadedState, gameOptions);
        }
        else if(gameOptions->displayTopTen){
            leaderBoard(0, NULL, -1);
            continueInput();
        }
        else if(gameOptions->exit){
            printf("Thank You for playing the game\n");
            break;
        }
        else {
            // this condition was made for debugging (unlikly to happen in the game).
            printf("error happened in the first gamee options input\n");
            break;
        }
    }

    return 0;
}

<<<<<<< HEAD
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
=======
void gameLoop(state * _state, options * gameOptions){
    if (_state == NULL){
>>>>>>> mainReWork
        // construct intial state
        _state = constructState(gameOptions->gridSize);
        _state->turn = 1; // 1 means player 1's turn, 2 means player 2 or AI turn
        // gameMode: 1 for human, 2 for AI easy, 3 for AI hard
        _state->gameMode = (gameOptions->gameMode == 1) ? (gameOptions->gameMode) : (gameOptions->gameMode + gameOptions->AIDifficulty - 1);
    }

    char playerWon = 0; // 0 means no one won, 1 means player 1 won, 2 means player 2 worn (AI is considered player 2)
    char * action;

    // construct undo and redo state
    undoRedo * undoRedo = constructUndoRedo(gameOptions->gridSize, copyState(_state));

    // someFlags for error messages
    char undoFlag = 0;
    char redoFlag = 0;
    char saveGameFlag = 0;
    char elseFlag = 0;

    while(!playerWon){
        displayState(_state);

        // error messages:
        if (saveGameFlag){
            printf(YELLOW"game saved successfully\n"RESET);
            saveGameFlag = 0;
        }
        if (undoFlag){
            printf(YELLOW"no undo available\n"RESET);
            undoFlag = 0;
        }
        if (redoFlag){
            printf(YELLOW"no redo available\n"RESET);
            redoFlag = 0;
        }

        // AI turn
        if (_state->turn == 2 && _state->gameMode != 1){
            char * action;
            
            if (_state->gameMode == 2)
                action = beginnerAi(_state);
            else 
                action = hardAIAction(_state);
            
            int actionFlag = applyStateAction(action, _state->turn, _state);
            playerWon = applyActionFlag(_state, undoRedo, actionFlag);
            if (playerWon)
                displayState(_state);
        }
        else {
            clock_t start = clock();
            char inGameMenu = displayInGameMenu(); // displays ingame menu and take input
            clock_t timeTillNow = clock();
            _state->time += (timeTillNow - start) / CLOCKS_PER_SEC;

            // palce a line
            if (inGameMenu == 1){
                char invalidActionFlag = 0;
                char actionFlag = 0;

                actionFlag = makeAnAction(_state, gameOptions);
                playerWon = applyActionFlag(_state, undoRedo, actionFlag);
            }

            // undo
            else if (inGameMenu == 2){
                state * newState = getUndo(undoRedo);
                undoFlag = validateUndoRedo(&_state, &newState);
            }

            // redo
            else if (inGameMenu == 3){
                state * newState = getRedo(undoRedo);
                redoFlag = validateUndoRedo(&_state, &newState);
            }

            // save game
            else if (inGameMenu == 4){
                char * availableSaveGameFiles = (char *)calloc(5, sizeof(char));
                checkAvailableStateFiles(saveGameFilesNames, availableSaveGameFiles);
                char fileIndex = displayAvailableFilesToSaveState(saveGameFilesNames, availableSaveGameFiles);
                if (fileIndex != 6){
                    saveStateToFile(saveGameFilesNames[fileIndex], _state);
                    saveGameFlag = 1;
                }
            }

            // go to main menu
            else {
                return;
            }
        }
    }

    displayState(_state);
    char winner = (playerWon == 3) ? 0 : playerWon; // 1 if player 1 win, 2 if player 2 or AI win, 0 Tie
    int winnerScore;
    if (playerWon == 1){
        winnerScore = _state->p1Score;
    }
    else if (playerWon == 2){
        winnerScore = _state->p2Score;
    }
    // for tie
    else {
        winnerScore = -1;
    }

    freeUndoRedo(undoRedo);
    freeState(_state);
    
    displayWinner(winner, gameOptions->gameMode);

    char * userName = NULL;
    if (winner == 1 || (winner == 2 && gameOptions->gameMode == 1)){
        printf("player %d Enter your name: ", playerWon);
        userName = takeUserName();
    }
    else winner = 0; // to make AI and Tie the same condition
    
    leaderBoard(winner, userName, winnerScore);
    if (userName != NULL)
        free(userName);

    continueInput();
}

char applyActionFlag(state * _state, undoRedo * _undoRedoStruct, int actionFlag){
    char playerWon = 0;
    if (actionFlag == 1){
        _state->turn = (_state->turn == 1) ? 2 : 1;
        if (_state->turn == 1 || (_state->turn == 2 && _state->gameMode == 1))
            pushStateToRedoUndo(_undoRedoStruct, copyState(_state));
    }
    // no need to check if actionFlag = 2 because if it is equal 2 I don't want to change anything.
    // no condtion for actionFlag = 0 because it is garanted that the AI will not make an invalid action.
    else if(actionFlag == 3){
        if (_state->p1Score > _state->p2Score){
            playerWon = 1;
        }
        else if (_state->p1Score < _state->p2Score){
            playerWon = 2;
        }
        else {
            playerWon = 3;
        }
    }
    return playerWon;
}

int makeAnAction(state * _state, options * gameOptions){
    char invalidActionFlag = 0; // used to break from the do-while loop
    char actionFlag = 0; // its value indicat the state of the game after the last action
    char * action = NULL; // stores the place where the user decided to play

    do {
        if (invalidActionFlag){
            printf("invalid action!\n");
            invalidActionFlag = 0;
        }

        printf("your input should be: \"row col side\" the sides are u for up, d for down, l for left, r for right\n");
        clock_t start = clock();
        action = makeMoveInput(gameOptions->gridSize);
        clock_t timeTillNow = clock();
        _state->time += (timeTillNow - start) / CLOCKS_PER_SEC;

        char chainFlag = checkChain(_state, action);
        if (chainFlag == 1){
            if (_state->numberOfRemainingCells == 0)
                actionFlag = 3;
            else 
                actionFlag = 2;
        }
        else if (chainFlag == 0)
            actionFlag = applyStateAction(action, _state->turn, _state);
        else 
            invalidActionFlag = 1;

        if (action != NULL)
            free(action);
            
    } while(invalidActionFlag);

    return actionFlag;
}

// if the return value of the undo or redo not null then update the 
char validateUndoRedo(state ** _state, state ** undoRedoState){
    if (*undoRedoState == NULL)
        return 1;
    
    (*undoRedoState)->time = (*_state)->time;
    freeState(*_state);
    *_state = *undoRedoState;
    return 0;
}

void displayWinner(int winner, char gameMode){
    if (winner == 3){
        printf(YELLOW"Tie\n"RESET);
    }
    else if (winner == 1){
        printf(RED"player 1\x1b[0m win\n");
    }
    else if (gameMode == 1){
        printf(BLUE"player 2\x1b[0m win\n");
    }
    else 
        printf(BLUE"AI\x1b[0m win\n");
}

void leaderBoard(char winner, char * playerName, int playerScore){
    scores * allScores = loadAndSortScores(leaderBoardFile);
    if (allScores == NULL){
        printf("The leader Board file is not found or empty or corrupted\n");
        if (winner == 0)
            return;
    }

    int index = -1;
    if (winner != 0){
        if (!userInScores(allScores, playerName, playerScore, &index)){
            allScores = addUserToScores(allScores, playerName, playerScore, &index);
        }
    }

    displayTopTen(allScores, index);
    if (winner != 0)
        saveScoresToFile(leaderBoardFile, allScores);
}

// this is used to freeze the screen
void continueInput(){
    printf("enter 1 to got to main menu\n");
    char t = mainMenuInput(1);
}

state * loadGame(options * gameOptions){
    state * _state = NULL;
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
        _state = loadStateFromFile(saveGameFilesNames[input - 1]);
        if (_state == NULL){
            fileIsBad = 1;
        }
    } while (fileIsBad);
    if (input != 6){
        gameOptions->gameMode = _state->gameMode;
        gameOptions->gridSize = _state->gridSize;
    }
    return _state;
}