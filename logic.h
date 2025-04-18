﻿#ifndef _LOGIC_H_
#define _LOGIC_H_
#include <SDL.h>
#include "defs.h"
#include "tetromino.h"

struct Logic {
    static bool isValid(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino);
    static void move(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino, int dx);
    static void rotateTetrominoClockwise(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino);
    static void rotateTetrominoAntiClockwise(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino);
    static void update(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino, bool& newTetrimino);
    static int checkLines(int grid[GRID_ROWS][GRID_COLS]);
    static void hardDrop(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino, bool& newTetrimino);
};

#endif _LOGIC_H_