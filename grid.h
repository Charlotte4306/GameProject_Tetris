
#ifndef _GRID_H
#define _GRID_H

#include <SDL.h>
#include <SDL_ttf.h>
void drawGrid(SDL_Renderer*);
void drawUI(SDL_Renderer*, TTF_Font*);
void drawPanel(SDL_Renderer* , TTF_Font*, int, int, int, int, const char*);
//void drawText(SDL_Renderer*, TTF_Font*, int, int, int, const char*, SDL_Color );
#endif _GRID_H
