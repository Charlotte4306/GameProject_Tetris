#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "grid.h"
#include "tetromino.h"
#include "logic.h"
#include "defs.h"

int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();
    Grid grid;
    Logic logic;
    logic.initLogic(&logic, &grid);
    SDL_Texture* background = graphics.loadTexture("Background/PIC2.JPG");
    
    TTF_Font* font = graphics.loadFont("Fonts/Courier.ttf", 50);
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    logic.moveTetromino(&logic, -1, 0);
                    break;
                case SDLK_RIGHT:
                    logic.moveTetromino(&logic, 1, 0);
                    break;
                case SDLK_UP:
                    logic.rotateTetromino(&logic);
                    break;
                case SDLK_DOWN:
                    logic.moveTetromino(&logic, 0, 1);
                    break;
                case SDLK_SPACE:
                    logic.dropTetromino(&logic);
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
            }
        }
        Uint32 currentTime = SDL_GetTicks();
        logic.updateLogic(&logic, currentTime);
        graphics.prepareScene(background);
        grid.drawGrid(graphics.renderer);
        grid.drawUI(graphics.renderer, font);
        for (int i = 0; i < GRID_ROWS; i++) {
            for (int j = 0; j < GRID_COLS; j++) {
                if (grid.grid[i][j] != 0) {
                    drawTetromino(graphics.renderer, grid.grid[i][j] - 1, GRID_X + j * CELL_SIZE, GRID_Y + i * CELL_SIZE, CELL_SIZE);
                }
            }
        }
        if (logic.currentTetromino != nullptr) {
            drawTetromino(graphics.renderer, logic.currentTetromino->type,
                GRID_X + logic.currentTetromino->x * CELL_SIZE,
                GRID_Y + logic.currentTetromino->y * CELL_SIZE,
                CELL_SIZE);
        }

        graphics.presentScene();
        SDL_Delay(16);
    }
    logic.freeLogic(&logic);
    graphics.quit();
	return 0;
}