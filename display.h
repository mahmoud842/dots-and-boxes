#ifndef display
#define display
#include "structures.h"

#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"

// options should be intialized with zeros
void displayMainMenu(options *startGame);
void initializeNearByCell(state *gridPosition, int row, int col , char s);
void displayState(state *gameState);
char displayInGameMenu();
char displayAvailableFilesForState(char fileNames[][14], char * availableFiles);

void displayTopTen(scores *topTenScores, int index);
#endif