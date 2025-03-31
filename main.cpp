#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "grid.h"
#include "logic.h"
#include "defs.h"
#include "menu.h"
#include "tetromino.h"
#include "score.h"
using namespace std;

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();
    Score score;
    Grid grid(score);
    srand(time(0));
    SDL_Texture* background = graphics.loadTexture("Background/PIC2.JPG");
    TTF_Font* titleFont = graphics.loadFont("Fonts/Courier.ttf", 80);
    TTF_Font* buttonFont = graphics.loadFont("Fonts/Courier.ttf", 50);
    TTF_Font* font = graphics.loadFont("Fonts/Courier.ttf", 40);
    Menu menu;
    menu.initMenu(&menu, &graphics, titleFont, buttonFont, background);
    GameState state = MENU;
    SDL_Event e;
    bool quit = false;
    Tetromino tetromino;
    tetromino.initTetromino();
    bool hardDropTriggered = false;
    bool holdTriggered = false; 
    bool canHold = true; 
    tetromino.nextTetromino(grid.nextTetrominos);
    Uint32 lastMoveTime = 0;
    const Uint32 moveDelay = 150; 
    const Uint32 initialMoveDelay = 200;
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
                    score.reset();
                    grid.reset();
                    tetromino.nextTetromino(grid.nextTetrominos);
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
                        case SDLK_LEFT: tetromino.dx = -1; break;
                        case SDLK_RIGHT: tetromino.dx = 1; break;
                        case SDLK_z: tetromino.rotate = true; break;
                        case SDLK_x: tetromino.rotate = true; break;
                        case SDLK_c: holdTriggered = true; break;
                        case SDLK_DOWN: tetromino.delay = 50; break;
                        case SDLK_SPACE: hardDropTriggered = true; break;
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                    }
                }
                if (e.type == SDL_KEYUP && (e.key.keysym.sym == SDLK_DOWN)) {
                    tetromino.delay = score.getDelay(); // Sử dụng delay từ Score
                }
            }
            else if (state == GAME_OVER) {
                menu.handleGameOverEvents(&menu, &e);
                if (menu.playAgainClicked) {
                    state = PLAYING;
                    menu.playAgainClicked = false;
                    score.reset();
                    tetromino.nextTetromino(grid.nextTetrominos);
                }
                if (menu.quitClicked) {
                    quit = true;
                    menu.quitClicked = false;
                }
            }
        }

        if (state == MENU) {
            menu.drawMenu(&menu);
        }
        else if (state == PLAYING) {
            tetromino.currentTime = SDL_GetTicks();
            graphics.prepareScene(background);
            grid.drawGrid(graphics.renderer);
            grid.drawUI(graphics.renderer, font);

            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT]) {
                int dx = 0;
                if (keystate[SDL_SCANCODE_LEFT]) dx = -1;
                if (keystate[SDL_SCANCODE_RIGHT]) dx = 1;
                Uint32 currentTime = SDL_GetTicks();
                Uint32 delay = (tetromino.dx == 0) ? initialMoveDelay : moveDelay;
                if (currentTime - lastMoveTime >= delay) {
                    Logic::move(grid.grid, tetromino, dx);
                    tetromino.dx = dx; 
                    lastMoveTime = currentTime;
                }
            }
            else {
                tetromino.dx = 0; 
            }
            if (tetromino.rotate) {
                const Uint8* keystate = SDL_GetKeyboardState(NULL);
                if (keystate[SDL_SCANCODE_Z]) {
                    Logic::rotateTetrominoAntiClockwise(grid.grid, tetromino); 
                }
                if (keystate[SDL_SCANCODE_X]) {
                    Logic::rotateTetrominoClockwise(grid.grid, tetromino);     
                }
                tetromino.rotate = false;
            }
            if (holdTriggered && canHold) {
                if (!grid.hasHeld) {
                    grid.heldTetromino = tetromino;
                    grid.hasHeld = true;
                    tetromino.nextTetromino(grid.nextTetrominos);
                }
                else {
                    Tetromino temp = tetromino;
                    tetromino = grid.heldTetromino;
                    grid.heldTetromino = temp;
                }
                canHold = false; 
                holdTriggered = false;
            }
            if (hardDropTriggered) {
                bool newTetrimino = false;
                Logic::hardDrop(grid.grid, tetromino, newTetrimino);
                if (newTetrimino) {
                    int lines = Logic::checkLines(grid.grid); // Cần sửa Logic::checkLines để trả về số dòng xóa
                    score.addLines(lines); // Cộng điểm và kiểm tra tăng level
                    if (!Logic::isValid(grid.grid, tetromino)) {
                        state = GAME_OVER;
                    }
                    else {
                        tetromino.nextTetromino(grid.nextTetrominos);
                        canHold = true;
                    }
                }
                hardDropTriggered = false;
            }
            else if (tetromino.currentTime - tetromino.startTime >= tetromino.delay) {
                bool newTetrimino = false;
                Logic::update(grid.grid, tetromino, newTetrimino);
                if (newTetrimino) {
                    int lines = Logic::checkLines(grid.grid); 
                    score.addLines(lines); // Cộng điểm và kiểm tra tăng level
                    if (!Logic::isValid(grid.grid, tetromino)) {
                        state = GAME_OVER;
                    }
                    else {
                        tetromino.nextTetromino(grid.nextTetrominos);
                        canHold = true;
                    }
                }
                tetromino.startTime = tetromino.currentTime;
            }
            else if (state == GAME_OVER) {
                menu.drawGameOver(&menu, score);
            }
            grid.drawTetrimino(graphics.renderer, tetromino);
            graphics.presentScene();
        }

        SDL_Delay(16);
    }
    menu.freeMenu(&menu);
    graphics.quit();
    return 0;
}