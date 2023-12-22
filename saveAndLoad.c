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