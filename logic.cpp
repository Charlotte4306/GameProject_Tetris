#include "logic.h"
#include <cstdlib>

void Logic::initLogic(Logic* logic, Grid* grid) {
    logic->grid = grid;
    logic->currentTetromino = nullptr;
    logic->lastFallTime = 0;
    logic->fallDelay = 500; 
    logic->gameOver = false;
    spawnTetromino(logic);
}

void Logic::freeLogic(Logic* logic) {
    if (logic->currentTetromino != nullptr) {
        free(logic->currentTetromino);
        logic->currentTetromino = nullptr;
    }
}

void Logic::spawnTetromino(Logic* logic) {
    if (logic->currentTetromino != nullptr) {
        free(logic->currentTetromino);
    }
    logic->currentTetromino = (Game*)malloc(sizeof(Game));
    logic->currentTetromino->type = rand() % 7;
    logic->currentTetromino->x = GRID_COLS / 2 - TETROMINO_SIZE / 2;
    logic->currentTetromino->y = 0;
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            logic->currentTetromino->shape[i][j] = Tetrominoes[logic->currentTetromino->type][i][j];
        }
    }
    if (checkCollision(logic, 0, 0)) {
        logic->gameOver = true;
    }
}

bool Logic::checkCollision(Logic* logic, int dx, int dy) {
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (logic->currentTetromino->shape[i][j]) {
                int newX = logic->currentTetromino->x + j + dx;
                int newY = logic->currentTetromino->y + i + dy;
                if (newX < 0 || newX >= GRID_COLS || newY >= GRID_ROWS) {
                    return true;
                }
                if (newY >= 0 && logic->grid->grid[newY][newX] != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Logic::moveTetromino(Logic* logic, int dx, int dy) {
    if (!checkCollision(logic, dx, dy)) {
        logic->currentTetromino->x += dx;
        logic->currentTetromino->y += dy;
    }
}

void Logic::rotateTetromino(Logic* logic) {
    int temp[TETROMINO_SIZE][TETROMINO_SIZE];
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            temp[i][j] = logic->currentTetromino->shape[TETROMINO_SIZE - 1 - j][i];
        }
    }
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            logic->currentTetromino->shape[i][j] = temp[i][j];
        }
    }
    if (checkCollision(logic, 0, 0)) {
        for (int k = 0; k < 3; k++) { 
            for (int i = 0; i < TETROMINO_SIZE; i++) {
                for (int j = 0; j < TETROMINO_SIZE; j++) {
                    temp[i][j] = logic->currentTetromino->shape[TETROMINO_SIZE - 1 - j][i];
                }
            }
            for (int i = 0; i < TETROMINO_SIZE; i++) {
                for (int j = 0; j < TETROMINO_SIZE; j++) {
                    logic->currentTetromino->shape[i][j] = temp[i][j];
                }
            }
        }
    }
}

void Logic::updateLogic(Logic* logic, Uint32 currentTime) {
    if (logic->gameOver) return;
    if (currentTime - logic->lastFallTime >= logic->fallDelay) {
        if (!checkCollision(logic, 0, 1)) {
            logic->currentTetromino->y++;
        }
        else {
            lockTetromino(logic);
            clearLines(logic);
            spawnTetromino(logic);
        }
        logic->lastFallTime = currentTime;
    }
}

void Logic::dropTetromino(Logic* logic) {
    while (!checkCollision(logic, 0, 1)) {
        logic->currentTetromino->y++;
    }
    lockTetromino(logic);
    clearLines(logic);
    spawnTetromino(logic);
}

void Logic::lockTetromino(Logic* logic) {
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (logic->currentTetromino->shape[i][j]) {
                int gridX = logic->currentTetromino->x + j;
                int gridY = logic->currentTetromino->y + i;
                if (gridY >= 0) {
                    logic->grid->grid[gridY][gridX] = logic->currentTetromino->type + 1; 
                }
            }
        }
    }
}

void Logic::clearLines(Logic* logic) {
    for (int i = GRID_ROWS - 1; i >= 0; i--) {
        bool full = true;
        for (int j = 0; j < GRID_COLS; j++) {
            if (logic->grid->grid[i][j] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < GRID_COLS; j++) {
                    logic->grid->grid[k][j] = logic->grid->grid[k - 1][j];
                }
            }
            for (int j = 0; j < GRID_COLS; j++) {
                logic->grid->grid[0][j] = 0;
            }
            i++;
        }
    }
}