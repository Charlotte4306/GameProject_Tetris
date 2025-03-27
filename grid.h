
#ifndef _GRID_H
#define _GRID_H

#include <SDL.h>

void drawGrid(SDL_Renderer*);
void drawUI(SDL_Renderer*);
void drawPanel(SDL_Renderer*, int, int, int, int, SDL_Color);
void drawText(SDL_Renderer*, int, int, const char*);
#endif _GRID_H
