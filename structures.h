#ifndef structures
#define structures

#define MAX_CHAR_OF_NAME 21

typedef struct cell{
    // any of the sides if equal to 1 then there is a line there, if 0 then there is no line.
    char up;
    char down;
    char right;
    char left;

    // 0 for no one, 1 if player 1 completed it, 2 if player 2 completed it.
    // so when printing it we know which color the space should be.
    char owner;
} cell;

typedef struct state{
    char gridSize; // diminsions
    char gameMode; // 0 for human, 1 for AI
    int time; // number of sec passed and then we will calculate the minutes from it.
    cell ** grid; // grid actuall size in bytes = sizeof(cell) * height * width (for loading and writing to files)

    char turn; // if 0 then player 1's turn, if 1 than the player 2's turn.

    // scores are made in char bec they can't pass 255 each;
    char p1Score;
    char p2Score;
    char p1Moves;
    char p2Moves;

    int numberOfRemainingCells; // number of remaining incomplete cells

} state;

typedef struct options{
    char start;  //starting a new game if 1 so it is chosen otherwise not chosen
    // the actuall used size from the grid.
    char gridSize;  //takes 2 for Beginner or 5 for expert 

    // 1 for player vs player, 2 for player vs AI.
    char gameMode;
    // for different levels of AI.
    // hasn't defined any levels for it yet.
    char AIDifficulty;  // 1 for easy 2 for hard maybe updated

    char loadGame;   //chose loaded game if 1 so it is chosen otherwise not chosen
    
    char displayTopTen;  // chose to display top 10 if 1 so it is chosen otherwise not chosen

    char exit;
} options;

// max size of a name is 20 chars (letters and spaces only)
typedef struct userScore{
    char * name;
    int score;
} userScore;

typedef struct scores{
    int numberOfUsers;
    userScore * usersScores;
} scores;

// constructors, intialize and free for each structure:
// scores:
void freeScores(scores * s);
scores * constructScores();

// cell:
cell * constructCell();
cell * intialize(cell * c, char u, char d, char r, char l, char own);
void freeCell(cell * c);

// state:
// size of grid = 5 if you want 5x5, size = 2 if you want 2x2 and so on.
state * constructState(int sizeOfGrid);
void freeState(state * s);

void intializeOptionsWith0(options * gameOptions);

#endif