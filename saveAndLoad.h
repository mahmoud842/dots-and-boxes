#ifndef saveAndLoad
#define saveAndLoad

#include <stdio.h>
#include "structures.h"

// this function checks if the available and then return the number of lines in it.
// this function is made to be used with the scores.csv file.
char checkFileAvailable(char * fileName);

scores * loadScores(char * fileName);

#endif