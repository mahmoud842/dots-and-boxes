#ifndef ai
#define ai

#include "structures.h"
#include <stdio.h>
int countCellSides(cell c);
char * generateActionOfAI(state * s, int i, int j, int score);
void dfs(state * s, char ** visited, int i, int j, int cameFrom, int directions[4][2], char * fail);
char checkChain(state * s, char * action);
char * beginnerAi(state *s);
char * hardAIAction(state * s);

#endif