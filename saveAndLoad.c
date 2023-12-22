#include "saveAndLoad.h"

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"


// return number of lines
char checkFileAvailable(char * fileName){
    FILE *file;
    if (file = fopen(fileName, "rb"))
    {
        int counter = 0;
        char nameBuffer[MAX_CHAR_OF_NAME];
        while (fread(nameBuffer, sizeof(char), MAX_CHAR_OF_NAME, file)){
            int tmp;
            fread(&tmp, sizeof(int), 1, file);
            counter++;
        }
        fclose(file);
        return counter;
    }
    return 0;
}

void addUserScoreToFile(char * fileName, char * userName, int score){
    FILE * file;
    if (file = fopen(fileName, "ab")){
        fwrite((void *)userName, sizeof(char) * MAX_CHAR_OF_NAME, 1, file);
        fwrite((void *)&score, sizeof(int), 1, file);
    }
    else {
        printf("%s \n", "error");
    }
    fclose(file);
}

// both strings are the same size. (MAX_CHAR_OF_NAME)
// copy right to left.
void copyStrToStr(char * s1, char * s2){
    for (int i = 0; i < MAX_CHAR_OF_NAME; i++){
        s1[i] = s2[i];
    }
}

// incomplete
// I need to check if this file has been corrupted.
// this function returns NULL if the file is corrupted or empty or doesn't exist.
scores * loadScoresFromFile(char * fileName){
    // num instead of number because numberOfUsers is defined in the scores structure.
    int numOfUsers = checkFileAvailable(fileName);
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