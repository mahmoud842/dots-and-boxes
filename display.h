#ifndef display
#define display
#include "structures.h"

// options should be intialized with zeros
void displayMainMenu(options *startGame);
void initializeNearByCell(state *gridPosition, int row, int col , char s);
void displayState(state *gameState);
char displayInGameMenu();
void displayTopTen(scores *topTenScores);
#endif