﻿#ifndef _GRID_H
#define _GRID_H
#include <SDL.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "tetromino.h"
#include <vector>
#include "score.h"
struct Grid {
    int grid[GRID_ROWS][GRID_COLS];
    Tetromino heldTetromino; 
    bool hasHeld; 
    Score& score;
    std::vector<Tetromino> nextTetrominos; 
    Grid(Score& score);
    void reset();
    void drawGrid(SDL_Renderer*);
    void drawUI(SDL_Renderer*, TTF_Font*);
    void drawPanel(SDL_Renderer*, TTF_Font*, int, int, int, int, const char*);
    void drawTetrimino(SDL_Renderer*, Tetromino&);
};

#endif _GRID_H