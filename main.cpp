#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "grid.h"
#include "logic.h"
#include "defs.h"
#include "menu.h"
#include "tetromino.h"
using namespace std;

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();
    Grid grid;
    srand(time(0));
    SDL_Texture* background = graphics.loadTexture("Background/PIC2.JPG");
    TTF_Font* titleFont = graphics.loadFont("Fonts/Courier.ttf", 80);
    TTF_Font* buttonFont = graphics.loadFont("Fonts/Courier.ttf", 50);
    TTF_Font* font = graphics.loadFont("Fonts/Courier.ttf", 60);
    Menu menu;
    menu.initMenu(&menu, &graphics, titleFont, buttonFont, background);
    GameState state = MENU;
    SDL_Event e;
    bool quit = false;
    Tetromino tetromino;
    tetromino.initTetromino();
    bool hardDropTriggered = false;
    bool holdTriggered = false; // Biến để theo dõi trạng thái hold
    bool canHold = true; // Biến kiểm tra xem có thể hold trong lượt này không
    tetromino.nextTetromino(grid.nextTetrominos);
    Uint32 lastMoveTime = 0;
    const Uint32 moveDelay = 100; // Độ trễ giữa các lần di chuyển (ms), giảm để nhanh hơn
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
                    tetromino.delay = 500;
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

                // Kiểm tra thời gian để điều khiển độ nhạy
                Uint32 currentTime = SDL_GetTicks();
                Uint32 delay = (tetromino.dx == 0) ? initialMoveDelay : moveDelay;
                if (currentTime - lastMoveTime >= delay) {
                    Logic::move(grid.grid, tetromino, dx);
                    tetromino.dx = dx; // Lưu dx để biết đang di chuyển
                    lastMoveTime = currentTime;
                }
            }
            else {
                tetromino.dx = 0; // Reset dx khi không nhấn phím
            }
            if (tetromino.rotate) {
                const Uint8* keystate = SDL_GetKeyboardState(NULL);
                if (keystate[SDL_SCANCODE_Z]) {
                    Logic::rotateTetrominoAntiClockwise(grid.grid, tetromino); // Z: Xoay ngược chiều
                }
                if (keystate[SDL_SCANCODE_X]) {
                    Logic::rotateTetrominoClockwise(grid.grid, tetromino);     // X: Xoay theo chiều
                }
                tetromino.rotate = false;
            }
            if (holdTriggered && canHold) {
                if (!grid.hasHeld) {
                    // Nếu chưa có khối nào trong HOLD, giữ khối hiện tại
                    grid.heldTetromino = tetromino;
                    grid.hasHeld = true;
                    tetromino.nextTetromino(grid.nextTetrominos);
                }
                else {
                    // Nếu đã có khối trong HOLD, hoán đổi
                    Tetromino temp = tetromino;
                    tetromino = grid.heldTetromino;
                    grid.heldTetromino = temp;
                }
                canHold = false; // Chỉ được hold 1 lần mỗi lượt
                holdTriggered = false;
            }
            if (hardDropTriggered) {
                bool newTetrimino = false;
                Logic::hardDrop(grid.grid, tetromino, newTetrimino);
                if (newTetrimino) {
                    Logic::checkLines(grid.grid);
                    tetromino.nextTetromino(grid.nextTetrominos);
                    canHold = true; // Reset khả năng hold khi khối mới xuất hiện
                }
                hardDropTriggered = false;
            }
            else if (tetromino.currentTime - tetromino.startTime >= tetromino.delay) {
                bool newTetrimino = false;
                Logic::update(grid.grid, tetromino, newTetrimino);
                if (newTetrimino) {
                    Logic::checkLines(grid.grid);
                    tetromino.nextTetromino(grid.nextTetrominos);
                    canHold = true; // Reset khả năng hold khi khối mới xuất hiện
                }
                tetromino.startTime = tetromino.currentTime;
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