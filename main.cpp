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
#include <vector>
using namespace std;

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();

    // Tải nhạc cố định cho menu
    graphics.loadMenuBackgroundMusic();
    graphics.loadBackgroundMusicList();

    // Hình nền cố định cho menu
    SDL_Texture* menuBackground = graphics.loadTexture("Background/BG.JPG");

    // Danh sách hình ảnh nền cho phần chơi
    vector<SDL_Texture*> backgroundList;
    for (int i = 1; i <= 10; i++) {
        std::string filePath = "Background/PIC" + std::to_string(i) + ".JPG";
        SDL_Texture* texture = graphics.loadTexture(filePath.c_str());
        if (texture != nullptr) {
            backgroundList.push_back(texture);
        }
    }

    // Hình nền và nhạc hiện tại
    SDL_Texture* currentBackground = menuBackground;
    int currentMusicIndex = -1; // -1 nghĩa là đang dùng nhạc menu
    bool isMenuMusicPlaying = false; // Theo dõi trạng thái nhạc menu

    // Phát nhạc menu lần đầu
    graphics.playMenuBackgroundMusic();
    isMenuMusicPlaying = true;

    srand(static_cast<unsigned int>(time(0)));
    Score score;
    Grid grid(score);

    TTF_Font* titleFont = graphics.loadFont("Fonts/Courier.ttf", 80);
    TTF_Font* buttonFont = graphics.loadFont("Fonts/Courier.ttf", 50);
    TTF_Font* font = graphics.loadFont("Fonts/Courier.ttf", 40);
    Menu menu;
    menu.initMenu(&menu, &graphics, titleFont, buttonFont, menuBackground);
    menu.initGameOverMenu(&menu, &graphics, titleFont, buttonFont, menuBackground);
    GameState state = MENU;
    GameState lastState = MENU; // Theo dõi trạng thái trước đó
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

                    // Random hình ảnh và nhạc cho phần chơi
                    if (!backgroundList.empty()) {
                        currentBackground = backgroundList[rand() % backgroundList.size()];
                        currentMusicIndex = rand() % 10;
                        graphics.playBackgroundMusic(currentMusicIndex);
                        isMenuMusicPlaying = false;
                    }
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
                    tetromino.delay = score.getDelay();
                }
            }
            else if (state == GAME_OVER) {
                menu.handleGameOverEvents(&menu, &e);
                if (menu.playAgainClicked) {
                    state = PLAYING;
                    menu.playAgainClicked = false;
                    score.reset();
                    grid.reset();
                    tetromino.nextTetromino(grid.nextTetrominos);

                    // Random hình ảnh và nhạc cho phần chơi
                    if (!backgroundList.empty()) {
                        currentBackground = backgroundList[rand() % backgroundList.size()];
                        currentMusicIndex = rand() % 10;
                        graphics.playBackgroundMusic(currentMusicIndex);
                        isMenuMusicPlaying = false;
                    }
                }
                if (menu.quitClicked) {
                    quit = true;
                }
            }
        }

        // Kiểm tra chuyển trạng thái để phát nhạc
        if (state != lastState) {
            if (state == MENU || state == GAME_OVER) {
                if (!isMenuMusicPlaying) {
                    currentBackground = menuBackground;
                    graphics.playMenuBackgroundMusic();
                    isMenuMusicPlaying = true;
                }
            }
            lastState = state;
        }

        if (state == MENU) {
            currentBackground = menuBackground;
            menu.drawMenu(&menu);
        }
        else if (state == PLAYING) {
            tetromino.currentTime = SDL_GetTicks();
            graphics.prepareScene(currentBackground);
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
                    int lines = Logic::checkLines(grid.grid);
                    score.addLines(lines);
                    tetromino.nextTetromino(grid.nextTetrominos);
                    canHold = true;
                    if (!Logic::isValid(grid.grid, tetromino)) {
                        state = GAME_OVER;
                    }
                }
                hardDropTriggered = false;
            }
            else if (tetromino.currentTime - tetromino.startTime >= tetromino.delay) {
                bool newTetrimino = false;
                Logic::update(grid.grid, tetromino, newTetrimino);
                if (newTetrimino) {
                    int lines = Logic::checkLines(grid.grid);
                    score.addLines(lines);
                    tetromino.nextTetromino(grid.nextTetrominos);
                    canHold = true;
                    if (!Logic::isValid(grid.grid, tetromino)) {
                        state = GAME_OVER;
                    }
                }
                tetromino.startTime = tetromino.currentTime;
            }

            tetromino.delay = score.getDelay();
            grid.drawTetrimino(graphics.renderer, tetromino);
            graphics.presentScene();
        }
        else if (state == GAME_OVER) {
            currentBackground = menuBackground;
            menu.drawGameOver(&menu, score);
        }

        SDL_Delay(16);
    }

    // Giải phóng các texture
    SDL_DestroyTexture(menuBackground);
    for (SDL_Texture* texture : backgroundList) {
        SDL_DestroyTexture(texture);
    }
    backgroundList.clear();

    menu.freeMenu(&menu);
    graphics.quit();
    return 0;
}