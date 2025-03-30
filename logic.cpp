#include "logic.h"
#include <cstdlib>

bool Logic::isValid(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino) {
    for (int i = 0; i < 4; i++) {
        if (tetromino.items[i].x < 0 || tetromino.items[i].x >= GRID_COLS || tetromino.items[i].y >= GRID_ROWS)
            return false;
        else if (tetromino.items[i].y >= 0 && grid[tetromino.items[i].y][tetromino.items[i].x])
            return false;
    }
    return true;
}

void Logic::move(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino, int dx) {
    for (int i = 0; i < 4; i++) {
        tetromino.backup[i] = tetromino.items[i];
        tetromino.items[i].x += dx;
    }
    if (!isValid(grid, tetromino)) {
        for (int i = 0; i < 4; i++)
            tetromino.items[i] = tetromino.backup[i];
    }
}

void Logic::rotateTetrominoClockwise(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino) {
    if (tetromino.type == 3) return;
    Point p = tetromino.items[2]; 
    for (int i = 0; i < 4; i++) {
        tetromino.backup[i] = tetromino.items[i];
        int x = tetromino.items[i].y - p.y;
        int y = tetromino.items[i].x - p.x;
        tetromino.items[i].x = p.x - x;
        tetromino.items[i].y = p.y + y;
    }
    if (!isValid(grid, tetromino)) {
        for (int i = 0; i < 4; i++)
            tetromino.items[i] = tetromino.backup[i];
    }
}

void Logic::rotateTetrominoAntiClockwise(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino) {
    if (tetromino.type == 3) return;
    Point p = tetromino.items[2]; 
    for (int i = 0; i < 4; i++) {
        tetromino.backup[i] = tetromino.items[i];
        int x = tetromino.items[i].y - p.y;
        int y = tetromino.items[i].x - p.x;
        tetromino.items[i].x = p.x + x; 
        tetromino.items[i].y = p.y - y; 
    }
    if (!isValid(grid, tetromino)) {
        for (int i = 0; i < 4; i++)
            tetromino.items[i] = tetromino.backup[i];
    }
}

void Logic::update(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino, bool& newTetrimino) {
    for (int i = 0; i < 4; i++)
        tetromino.backup[i] = tetromino.items[i];
    for (int i = 0; i < 4; i++)
        tetromino.items[i].y++;
    if (!isValid(grid, tetromino)) {
        for (int i = 0; i < 4; i++) {
            if (tetromino.backup[i].y >= 0)
                grid[tetromino.backup[i].y][tetromino.backup[i].x] = tetromino.color;
        }
        newTetrimino = true;
    }
}

void Logic::hardDrop(int grid[GRID_ROWS][GRID_COLS], Tetromino& tetromino, bool& newTetrimino) {
    while (true) {
        for (int i = 0; i < 4; i++) {
            tetromino.backup[i] = tetromino.items[i];
            tetromino.items[i].y++;
        }
        if (!isValid(grid, tetromino)) {
            for (int i = 0; i < 4; i++) {
                tetromino.items[i] = tetromino.backup[i];
                if (tetromino.backup[i].y >= 0)
                    grid[tetromino.backup[i].y][tetromino.backup[i].x] = tetromino.color;
            }
            newTetrimino = true;
            break;
        }
    }
}

void Logic::checkLines(int grid[GRID_ROWS][GRID_COLS]) {
    int k = GRID_ROWS - 1;
    for (int i = GRID_ROWS - 1; i >= 0; i--) {
        int count = 0;
        for (int j = 0; j < GRID_COLS; j++) {
            if (grid[i][j]) count++;
            grid[k][j] = grid[i][j];
        }
        if (count < GRID_COLS) k--;
    }
}