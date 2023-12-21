#include "saveAndLoad.h"

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

#define BUFFERS_MAX_SIZE 200

// return number of lines
char checkFileAvailable(char * fileName){
    FILE *file;
    if ((file = fopen(fileName, "r")))
    {
        int counter = 0;
        char buffer[40];
        while (fgets(buffer, 40, file))
            counter++;
        fclose(file);
        return counter;
    }
    return 0;
}

char toLowerCase(char c){
    if (c >= 'A' && c <= 'Z'){
        return c + 32;
    }
    return c;
}

char isLetterOrSpace(char c){
    if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == ' ')
        return 1;
    return 0;
}

// incomplete
// I need to check if this file has been corapted.
scores * loadScores(char * fileName){
    // num instead of number because numberOfUsers is defined in the scores structure.
    int numOfUsers = checkFileAvailable(fileName);
    if (numOfUsers == 0) return NULL;

    // construct the scores:
    scores * scoresPtr = constructScores(numOfUsers);

    FILE * file;
    file = fopen(fileName, "r");
    char buffer[40];
    int userIndex = 0;
    while (fgets(buffer, 40, file)){
        int p = 0;
        char scoreBUFF[] = "000000";
        int i = 0;
        
        // some flags to clean data if corapted.
        char spaceFlag = 1;

        for (; buffer[p] != ','; p++){
            if (!isLetterOrSpace(buffer[p])) continue;
            if (spaceFlag && buffer[p] == ' ') continue;

            if (isLetterOrSpace(buffer[p])){
                scoresPtr->usersScores[userIndex].name[i] = toLowerCase(buffer[p]);
            }
            i++;
        }

    }
}