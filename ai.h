#ifndef ai
#define ai

#include "structures.h"
#include <stdio.h>

void dfs(state * s, char ** visited, int i, int j, int cameFrom, int directions[4][2], char * fail);
char checkChain(state * s, char * action);
char beginnerAi(state *s);

#endif