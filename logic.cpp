#include "logic.h"
#include <cstdlib>
#include "tetromino.h"

void Logic::initLogic(Logic* logic, Grid* grid) {
    logic->grid = grid;
    logic->currentTetromino = nullptr;
    logic->holdTetromino = nullptr;
    logic->nextIndex = 0;
    logic->lastFallTime = 0;
    logic->fallDelay = 500;
    logic->gameOver = false;
    logic->canHold = true;
    for (int i = 0; i < 5; i++) {
        logic->nextTetrominoes[i] = rand() % 7;
    }
    spawnTetromino(logic);
}

void Logic::freeLogic(Logic* logic) {
    if (logic->currentTetromino != nullptr) {
        free(logic->currentTetromino);
        logic->currentTetromino = nullptr;
    }
    if (logic->holdTetromino != nullptr) {
        free(logic->holdTetromino);
        logic->holdTetromino = nullptr;
    }
}

void Logic::spawnTetromino(Logic* logic) {
    if (logic->currentTetromino != nullptr) {
        free(logic->currentTetromino);
    }
    logic->currentTetromino = (Game*)malloc(sizeof(Game));
    logic->currentTetromino->type = logic->nextTetrominoes[logic->nextIndex];
    logic->currentTetromino->x = GRID_COLS / 2 - TETROMINO_SIZE / 2;
    logic->currentTetromino->y = -1;
    logic->currentTetromino->rotation = 0; 
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            logic->currentTetromino->shape[i][j] = Tetrominoes[logic->currentTetromino->type][logic->currentTetromino->rotation][i][j];
        }
    }
    logic->nextIndex = (logic->nextIndex + 1) % 5;
    if (logic->nextIndex == 0) {
        for (int i = 0; i < 5; i++) {
            logic->nextTetrominoes[i] = rand() % 7;
        }
    }
    if (checkCollision(logic, 0, 0)) {
        logic->currentTetromino->y--;  
        if (checkCollision(logic, 0, 0)) {
            logic->gameOver = true;  
        }
    }
    logic->canHold = true;
}

bool Logic::checkCollision(Logic* logic, int dx, int dy) {
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            if (logic->currentTetromino->shape[i][j]) {
                int newX = logic->currentTetromino->x + j + dx;
                int newY = logic->currentTetromino->y + i + dy;
                if (newX < 0 || newX >= GRID_WIDTH) {
                    return true;
                }
                if (newY >= GRID_HEIGHT) {
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
    int oldRotation = logic->currentTetromino->rotation;
    int oldShape[TETROMINO_SIZE][TETROMINO_SIZE];
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            oldShape[i][j] = logic->currentTetromino->shape[i][j];
        }
    }
    logic->currentTetromino->rotation = (logic->currentTetromino->rotation + 1) % 4;
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            logic->currentTetromino->shape[i][j] = Tetrominoes[logic->currentTetromino->type][logic->currentTetromino->rotation][i][j];
        }
    }
    if (checkCollision(logic, 0, 0)) {
        if (!checkCollision(logic, -1, 0)) {
            logic->currentTetromino->x--;
        }
        else if (!checkCollision(logic, 1, 0)) {
            logic->currentTetromino->x++;
        }
        else if (!checkCollision(logic, -2, 0)) {
            logic->currentTetromino->x -= 2;
        }
        else if (!checkCollision(logic, 2, 0)) {
            logic->currentTetromino->x += 2;
        }
        else {
            logic->currentTetromino->rotation = oldRotation;
            for (int i = 0; i < TETROMINO_SIZE; i++) {
                for (int j = 0; j < TETROMINO_SIZE; j++) {
                    logic->currentTetromino->shape[i][j] = oldShape[i][j];
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
            i--;  
        }
    }
}


void Logic::holdTe(Logic* logic) {
    if (!logic->canHold) return;

    if (logic->holdTetromino == nullptr) {
        logic->holdTetromino = new Game();
        logic->holdTetromino->type = logic->currentTetromino->type;
        logic->holdTetromino->rotation = 0;
        for (int i = 0; i < TETROMINO_SIZE; i++) {
            for (int j = 0; j < TETROMINO_SIZE; j++) {
                logic->holdTetromino->shape[i][j] = Tetrominoes[logic->holdTetromino->type][logic->holdTetromino->rotation][i][j];
            }
        }
        spawnTetromino(logic);
    }
    else {
        int tempType = logic->holdTetromino->type;
        logic->holdTetromino->type = logic->currentTetromino->type;
        logic->holdTetromino->rotation = 0;
        logic->currentTetromino->type = tempType;
        logic->currentTetromino->rotation = 0;

        for (int i = 0; i < TETROMINO_SIZE; i++) {
            for (int j = 0; j < TETROMINO_SIZE; j++) {
                logic->currentTetromino->shape[i][j] = Tetrominoes[logic->currentTetromino->type][logic->currentTetromino->rotation][i][j];
            }
        }
        for (int i = 0; i < TETROMINO_SIZE; i++) {
            for (int j = 0; j < TETROMINO_SIZE; j++) {
                logic->holdTetromino->shape[i][j] = Tetrominoes[logic->holdTetromino->type][logic->holdTetromino->rotation][i][j];
            }
        }

        logic->currentTetromino->x = GRID_COLS / 2 - TETROMINO_SIZE / 2;
        logic->currentTetromino->y = -1;  
    }
    logic->canHold = false;
}
