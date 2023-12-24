#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "inputValidation.h"
#include "display.h"
#include "saveAndLoad.h"

// compile command:
// gcc main.c structures.c inputValidation.c display.c saveAndLoad.c -o dotsAndLines
// run: ./dotsAndLines

int main(){

    state *myState = constructState(5);
    char *move ;
    
   
    for (int i = 0; i < 4; i++)
    {
        myState->turn = 1;
        move = makeMoveInput(5);
        initializeNearByCell(myState,move[0]-1, move[1]-1 , move[2]);
        
        
        
    }
        for (int i = 0; i < 4; i++)
    {
        myState->turn = 0;
        move = makeMoveInput(5);
        initializeNearByCell(myState,move[0]-1, move[1]-1 , move[2]);
        
        
        
    }
    
    displayState(myState);
    return 0;
}