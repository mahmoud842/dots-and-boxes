#ifndef display
#define display
#include "structures.h"
void displayMainMenu(options *startGame);
void initializeNearByCell(state *gridPosition, int row, int col , char s);
void displayState(state *gameState);

#endif