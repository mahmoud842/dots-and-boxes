#ifndef saveAndLoad
#define saveAndLoad

#include <stdio.h>
#include "structures.h"

#define NUM_SAVEGAME_FILES 5

// this function checks if the available and then return the number of lines in it.
// this function is made to be used with the scores.csv file.
int checkScoresFileAvailable(char * fileName);
char checkScoresFileCorruption(char * fileName);
void addUserScoreToFile(char * fileName, char * userName, int score);
scores * loadScoresFromFile(char * fileName);
scores * loadAndSortScores(char * fileName);
char saveScoresToFile(char * fileName, scores * _scores);
char * checkAvailableStateFiles(char ** _fileNames, char * _availableSaveGameFiles);
char * saveStateToFile(char * _fileName, state * _state);
state * loadStateFromFile(char * _fileName);
char * checkStateFileCorrupted(char * _fileName);

#endif