#ifndef _DEFS_H
#define _DEFS_H
#include <windows.h>
const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
const int CELL_SIZE = 60;     
const int GRID_COLS = 10;      
const int GRID_ROWS = 20;      
const int GRID_WIDTH = GRID_COLS * CELL_SIZE;
const int GRID_HEIGHT = GRID_ROWS * CELL_SIZE;
const int GRID_X = (SCREEN_WIDTH - GRID_WIDTH) / 2;
const int GRID_Y = (SCREEN_HEIGHT - GRID_HEIGHT) / 2;
const int PANEL_WIDTH = 6 * CELL_SIZE; 
const int PANEL_HEIGHT = 6 * CELL_SIZE;
const int HOLD_X = GRID_X - PANEL_WIDTH - 20;
const int HOLD_Y = GRID_Y;
const int NEXT_X = GRID_X + GRID_WIDTH + 20;
const int NEXT_Y = GRID_Y;

#endif _DEFS_H