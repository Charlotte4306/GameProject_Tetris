#ifndef _TETROMINO_H
#define _TETROMINO_H
#include <SDL.h>
const int TETROMINO_SIZE = 4;
extern const int Tetrominoes[7][4][TETROMINO_SIZE][TETROMINO_SIZE]; // I O T S Z J L
void drawTetromino(SDL_Renderer*, int,int, int, int, int);


#endif _TETROMINO_H
