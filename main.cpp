#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "grid.h"
#include "tetromino.h"
#include "logic.h"
#include "defs.h"
#include "menu.h"
enum GameState {
    MENU,
    PLAYING
};
int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();
    Grid grid;
    Logic logic;
    logic.initLogic(&logic, &grid);
    SDL_Texture* background = graphics.loadTexture("Background/PIC2.JPG");
    TTF_Font* titleFont = graphics.loadFont("Fonts/Courier.ttf", 80); 
    TTF_Font* buttonFont = graphics.loadFont("Fonts/Courier.ttf", 50);
    TTF_Font* font = graphics.loadFont("Fonts/Courier.ttf", 60);
    Menu menu;
    menu.initMenu(&menu, &graphics, titleFont, buttonFont, background);
    GameState state = MENU;
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (state == MENU) {
                menu.handleMenuEvents(&menu, &e);
                if (menu.playClicked) {
                    state = PLAYING; 
                    menu.playClicked = false;
                }
                if (menu.settingsClicked) {
                    std::cout << "Settings clicked!" << std::endl;
                    menu.settingsClicked = false;
                }
                if (menu.helpClicked) {
                    std::cout << "Help clicked!" << std::endl;
                    menu.helpClicked = false;
                }
            }
            else if (state == PLAYING) {
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        logic.moveTetromino(&logic, -1, 0);
                        break;
                    case SDLK_RIGHT:
                        logic.moveTetromino(&logic, 1, 0);
                        break;
                    case SDLK_z:
                        logic.rotateTetromino(&logic);
                        break;
                    case SDLK_DOWN:
                        logic.moveTetromino(&logic, 0, 1);
                        break;
                    case SDLK_SPACE:
                        logic.dropTetromino(&logic);
                        break;
                    case SDLK_c: 
                        logic.holdTe(&logic);
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    }
                }
            }
        }
        if (state == MENU) {
            menu.drawMenu(&menu);
        }
        else if (state == PLAYING) {
            Uint32 currentTime = SDL_GetTicks();
            logic.updateLogic(&logic, currentTime);
            graphics.prepareScene(background);
            grid.drawGrid(graphics.renderer);
            grid.drawUI(graphics.renderer, font);
            Game* currentTetromino = logic.currentTetromino;
            for (int i = 0; i < GRID_ROWS; i++) {
                for (int j = 0; j < GRID_COLS; j++) {
                    if (grid.grid[i][j] != 0) {
                        drawTetromino(graphics.renderer, 0, grid.grid[i][j] - 1, GRID_X + j * CELL_SIZE, GRID_Y + i * CELL_SIZE, CELL_SIZE);
                    }
                }
            }
            if (currentTetromino != nullptr) {
                    drawTetromino(graphics.renderer, currentTetromino->type, currentTetromino->rotation,
                    GRID_X + currentTetromino->x * CELL_SIZE,
                    GRID_Y + currentTetromino->y * CELL_SIZE,
                    CELL_SIZE);
            }
            Game* holdTetromino = logic.holdTetromino;
            if (holdTetromino != nullptr) {
                drawTetromino(graphics.renderer, holdTetromino->type, 0,
                    HOLD_X + CELL_SIZE, HOLD_Y + 2 * CELL_SIZE, CELL_SIZE / 2);
            }

            const int NEXT_CELL_SIZE = CELL_SIZE / 2; 
            const int NEXT_TETROMINO_HEIGHT = NEXT_CELL_SIZE * 4; 
            const int NEXT_START_Y = NEXT_Y + CELL_SIZE + 10; 
            for (int i = 0; i < 4; i++) {
                int nextType = logic.nextTetrominoes[(logic.nextIndex + i) % 5];
                drawTetromino(graphics.renderer, nextType, 0,
                    NEXT_X + NEXT_CELL_SIZE, NEXT_START_Y + i * (NEXT_TETROMINO_HEIGHT + 10), NEXT_CELL_SIZE);
            }

            graphics.presentScene();
        }
        SDL_Delay(16);
    }
    menu.freeMenu(&menu);
    logic.freeLogic(&logic);
    graphics.quit();
	return 0;
}