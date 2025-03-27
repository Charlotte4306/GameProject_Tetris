#ifndef _LOGIC_H
#define _LOGIC_H
#include <SDL.h>
#include "grid.h"
#include "defs.h"
#include "tetromino.h"
struct Game {
	int type;
	int x, y;
	int shape[TETROMINO_SIZE][TETROMINO_SIZE];
};
struct Logic {
    Grid* grid; 
    Game* currentTetromino; 
    Uint32 lastFallTime; 
    Uint32 fallDelay; 
    void initLogic(Logic*, Grid*);
    void freeLogic(Logic*);
    void spawnTetromino(Logic*);
    bool checkCollision(Logic*, int, int);
    void moveTetromino(Logic*, int, int);
    void rotateTetromino(Logic*);
    void updateLogic(Logic*, Uint32);
    void dropTetromino(Logic*); 
    void lockTetromino(Logic*);
    void clearLines(Logic*);
    bool gameOver;
};
#endif _LOGIC_H