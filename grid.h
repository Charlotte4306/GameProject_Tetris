
#ifndef _GRID_H
#define _GRID_H
#include <SDL.h>
#include <SDL_ttf.h>
#include "defs.h"
struct Grid {
    int grid[GRID_ROWS][GRID_COLS];
    Grid();
    void drawGrid(SDL_Renderer*);
    void drawUI(SDL_Renderer*, TTF_Font*);
    void drawPanel(SDL_Renderer*, TTF_Font*, int, int, int, int, const char*);
};

#endif _GRID_H
