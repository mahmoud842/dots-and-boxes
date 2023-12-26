#include "saveAndLoad.h"

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"


// this function returns 1 if the file is corrupted or doesn't exist
char checkScoresFileCorruption(char * fileName){
    FILE * file;
    if(file = fopen(fileName, "rb")){
        fseek(file, 0, SEEK_END);
        long size = ftell(file);   // Get the current position (file size)
        fseek(file, 0, SEEK_SET);  // Reset the file position indicator to the beginning

        // (size - 4) the check sum size and (MAX_CHAR.. + 4) the total size of name and socre.
        if ((size - 4) % (MAX_CHAR_OF_NAME + 4) != 0 || size == 0){
            fclose(file);
            return 1;
        }

        int checksum = 0;
        int tmp = (size - 4) / (MAX_CHAR_OF_NAME + 4);
        int tmpScore;
        for (int i = 0; i < tmp; i++){
            fseek(file, MAX_CHAR_OF_NAME, SEEK_CUR);
            fread(&tmpScore, sizeof(int), 1, file);
            checksum ^= tmpScore;
        }

        int fileCheckSum;
        fread(&fileCheckSum, sizeof(int), 1, file);
        fclose(file);
        if (fileCheckSum != checksum){
            return 1;
        }
        return 0;
    }
    return 1;
}

// return number of lines
int checkScoresFileAvailable(char * fileName){
    if (checkScoresFileCorruption(fileName)){
        return 0;
    }

    FILE * file;
    file = fopen(fileName, "rb");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);   // Get the current position (file size)
    fclose(file);
    return (size - 4) / (MAX_CHAR_OF_NAME + 4);
}

// will be used if the number of users is less than the max number in the leader board
void addUserScoreToFile(char * fileName, char * userName, int score){
    char checksumFlag = 1; // 1 means there is a check sum and i can seek -4
    if (checkScoresFileCorruption(fileName)){
        checksumFlag = 0;
    }

    FILE * file;
    int checksum = 0;
    // read the checksum first if there
    if (checksumFlag){
        file = fopen(fileName, "rb");
        // Move the file position indicator to 4 bytes before the end
        fseek(file, -4, SEEK_END);
        fread(&checksum, sizeof(int), 1, file);
        fclose(file);
    }

    if (checksumFlag){
        file = fopen(fileName, "r+b");
        fseek(file, -4, SEEK_END);
    }
    else {
        file = fopen(fileName, "ab");
        fseek(file, 0, SEEK_END);
    }
    fwrite((void *)userName, sizeof(char) * MAX_CHAR_OF_NAME, 1, file);
    fwrite((void *)&score, sizeof(int), 1, file);
    checksum ^= score;
    fwrite(&checksum, sizeof(int), 1, file);
    fclose(file);
    
}

// both strings are the same size. (MAX_CHAR_OF_NAME)
// copy right to left.
void copyStrToStr(char * s1, char * s2){
    for (int i = 0; i < MAX_CHAR_OF_NAME; i++){
        s1[i] = s2[i];
    }
}

scores * loadScoresFromFile(char * fileName){
    // num instead of number because numberOfUsers is defined in the scores structure.
    int numOfUsers = checkScoresFileAvailable(fileName);
    if (numOfUsers == 0) return NULL;

    // construct the scores:
    scores * scoresPtr = constructScores(numOfUsers);

    FILE * file;
    file = fopen(fileName, "rb");

    for (int i = 0; i < numOfUsers; i++){
        char nameBuffer[MAX_CHAR_OF_NAME];
        int tmpScore;
        fread((void *)nameBuffer, sizeof(char), MAX_CHAR_OF_NAME, file);
        fread((void *)&tmpScore, sizeof(int), 1, file);

        copyStrToStr(scoresPtr->usersScores[i].name, nameBuffer);
        scoresPtr->usersScores[i].score = tmpScore; 
    }
    fclose(file);
    return scoresPtr;
    
}

void swapUserScores(userScore * ptr1, userScore * ptr2){
    userScore * tmp = (userScore *)malloc(sizeof(userScore));
    tmp->name = ptr1->name;
    tmp->score = ptr1->score;

    ptr1->name = ptr2->name;
    ptr1->score = ptr2->score;

    ptr2->name = tmp->name;
    ptr2->score = tmp->score;

    free(tmp);
}

// this function load and sort the scores from the file and if the file doesn't exist or corrupted or empty returns NULL.
scores * loadAndSortScores(char * fileName){
    scores * s = loadScoresFromFile(fileName);
    if (s == NULL) return NULL;
    for (int i = 0; i < s->numberOfUsers - 1; i++){
        for (int j = 0; j < s->numberOfUsers - i - 1; j++){
            if (s->usersScores[j].score < s->usersScores[j + 1].score){
                swapUserScores(&(s->usersScores[j]), &(s->usersScores[j + 1]));
            }
        }
    }
    return s;
}

char saveScoresToFile(char * fileName, scores * _scores){
    int checksum = 0;
    int n = _scores->numberOfUsers;
    FILE * file = fopen(fileName, "wb");
    for (int i = 0; i < n; i++){
        fwrite(_scores->usersScores[i].name, sizeof(char), MAX_CHAR_OF_NAME, file);
        fwrite(&_scores->usersScores[i].score, sizeof(int), 1, file);
        checksum ^= _scores->usersScores[i].score;
    }
    fwrite(&checksum, sizeof(int), 1, file);
    fclose(file);
}

// savegamei
char * checkAvailableStateFiles(char _fileNames[][14], char * _availableSaveGameFiles){
    FILE * file;
    int count = 0;
    for (int i = 0; i < NUM_SAVEGAME_FILES; i++){
        file = fopen(_fileNames[i], "rb");
        if (file != NULL){
            count++;
            _availableSaveGameFiles[i] = 1;
            fclose(file);
        }
        else{
            _availableSaveGameFiles[i] = 0;
        }
    }
    return 0;
}

char saveStateToFile(char * _fileName, state * _state){
    FILE * file = fopen(_fileName, "wb");
    if (file != NULL){
        // checksum is calculated and saved in the end of the file to check for corruption.
        int checksum = 0;
        checksum ^= _state->gridSize;
        fwrite(&(_state->gridSize), sizeof(char), 1, file);

        checksum ^= _state->gameMode;
        fwrite(&(_state->gameMode), sizeof(char), 1, file);

        checksum ^= _state->time;
        fwrite(&(_state->time), sizeof(int), 1, file);

        for (int i = 0; i < _state->gridSize; i++){
            for (int j = 0; j < _state->gridSize; j++){
                checksum ^= _state->grid[i][j].up;
                checksum ^= _state->grid[i][j].down;
                checksum ^= _state->grid[i][j].right;
                checksum ^= _state->grid[i][j].left;
                checksum ^= _state->grid[i][j].owner;
                fwrite(&(_state->grid[i][j]), sizeof(cell), 1, file);
            }
        }

        checksum ^= _state->turn;
        fwrite(&(_state->turn), sizeof(char), 1, file);

        checksum ^= _state->p1Score;
        fwrite(&(_state->p1Score), sizeof(char), 1, file);

        checksum ^= _state->p2Score;
        fwrite(&(_state->p2Score), sizeof(char), 1, file);

        checksum ^= _state->p1Moves;
        fwrite(&(_state->p1Moves), sizeof(char), 1, file);

        checksum ^= _state->p2Moves;
        fwrite(&(_state->p2Moves), sizeof(char), 1, file);

        checksum ^= _state->numberOfRemainingCells;
        fwrite(&(_state->numberOfRemainingCells), sizeof(int), 1, file);

        fwrite(&checksum, sizeof(int), 1, file);
        fclose(file);
        printf("saved checksum = %d\n", checksum);
        return 1; // success
    }
    else {
        return 0; // fail
    }
}

// size of state if (gridSize = 5) is 144
// size of state if (gridSize = 2) is 39
// if state is corrupted return NULL.
state * loadStateFromFile(char * _fileName){
    FILE * file = fopen(_fileName, "rb");
    if (file != NULL){
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        int checksum = 0;
        char sizeOfGrid = 0;
        fread(&sizeOfGrid, sizeof(char), 1, file);
        printf("file size = %d, sizeOfGrid = %d \n", fileSize, sizeOfGrid);

        if ( !(sizeOfGrid == 2 || sizeOfGrid == 5) || (sizeOfGrid == 2 && fileSize != 39) || (sizeOfGrid == 5 && fileSize != 144)){
            fclose(file);
            return NULL;
        }

        state * _state = constructState(sizeOfGrid);
        checksum ^= sizeOfGrid;

        fread(&(_state->gameMode), sizeof(char), 1, file);
        checksum ^= _state->gameMode;

        fread(&(_state->time), sizeof(int), 1, file);
        checksum ^= _state->time;

        for (int i = 0; i < _state->gridSize; i++){
            for (int j = 0; j < _state->gridSize; j++){
                fread(&(_state->grid[i][j]), sizeof(cell), 1, file);
                checksum ^= _state->grid[i][j].up;
                checksum ^= _state->grid[i][j].down;
                checksum ^= _state->grid[i][j].right;
                checksum ^= _state->grid[i][j].left;
                checksum ^= _state->grid[i][j].owner;
            }
        }

        fread(&(_state->turn), sizeof(char), 1, file);
        checksum ^= _state->turn;

        fread(&(_state->p1Score), sizeof(char), 1, file);
        checksum ^= _state->p1Score;

        fread(&(_state->p2Score), sizeof(char), 1, file);
        checksum ^= _state->p2Score;

        fread(&(_state->p1Moves), sizeof(char), 1, file);
        checksum ^= _state->p1Moves;

        fread(&(_state->p2Moves), sizeof(char), 1, file);
        checksum ^= _state->p2Moves;

        fread(&(_state->numberOfRemainingCells), sizeof(int), 1, file);
        checksum ^= _state->numberOfRemainingCells;

        int fileCheckSum;
        fread(&fileCheckSum, sizeof(int), 1, file);
        printf("file checksum = %d, calculated checksum = %d\n", fileCheckSum, checksum);
        if (fileCheckSum != checksum){
            freeState(_state);
            fclose(file);
            return NULL;
        }
        else {
            fclose(file);
            return _state;
        }
    }
    
    return NULL;
}

// copy left to right
void copyStr(char * name1, char * name2){
    for (int i = 0; i < MAX_CHAR_OF_NAME - 1; i++){
        name2[i] = name1[i];
    }
    name2[MAX_CHAR_OF_NAME - 1] = '\0';
}

// should return 1 if the user in the leader board the the board is updated and 0 otherwise
// but now the function doesn't have a max number of 10
scores * addUserToScores(scores * s, char * userName, char score){
    if (s == NULL){
        s = constructScores(1);
        copyStr(userName, s->usersScores[0].name);
        s->usersScores[0].score = score;
        return s;
    }

    scores * newScores = constructScores(s->numberOfUsers + 1);
    char userInserted = 0;

    int i = 0; // points on new score
    int p = 0; // points on old score 
    for (; i < newScores->numberOfUsers; i++){
        if ((i == newScores->numberOfUsers - 1 && !userInserted) || (s->usersScores[p].score < score && !userInserted)){
            newScores->usersScores[i].score = score;
            newScores->usersScores[i].name = userName;
        }
        else {
            copyStr(newScores->usersScores[i].name, s->usersScores[p].name);
            newScores->usersScores[i].score = s->usersScores[p].score;
            p++;
        }
    }

    freeScores(s);
    return newScores;

}