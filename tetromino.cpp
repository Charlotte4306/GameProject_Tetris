#include "tetromino.h"
#include <cstdlib>
#include <algorithm>

const int Tetromino::figures[7][4] = {
    {0, 1, 2, 3}, // I
    {0, 4, 5, 6}, // J
    {2, 6, 5, 4}, // L
    {1, 2, 5, 6}, // O
    {2, 1, 5, 4}, // S
    {1, 4, 5, 6}, // T
    {0, 1, 5, 6}  // Z
};

void Tetromino::initTetromino() {
    color = 0;
    type = 0;
    dx = 0;
    rotate = false;
    delay = 500;
    startTime = 0;
    currentTime = 0;
}

void Tetromino::nextTetromino(std::vector<Tetromino>& nextTetrominos) {
    if (nextTetrominos.empty()) {
        for (int i = 0; i < 3; i++) {
            Tetromino next;
            next.type = rand() % 7;
            next.color = next.type + 1;
            for (int j = 0; j < 4; j++) {
                next.items[j].x = figures[next.type][j] % 4;
                next.items[j].y = figures[next.type][j] / 4;
            }
            nextTetrominos.push_back(next);
        }
    }
    *this = nextTetrominos.front();
    nextTetrominos.erase(nextTetrominos.begin());
    Tetromino next;
    next.type = rand() % 7;
    next.color = next.type + 1;
    for (int j = 0; j < 4; j++) {
        next.items[j].x = figures[next.type][j] % 4;
        next.items[j].y = figures[next.type][j] / 4;
    }
    nextTetrominos.push_back(next);

    // Dịch chuyển Tetromino hiện tại để căn giữa theo chiều ngang
    int minX = items[0].x, maxX = items[0].x;
    for (int i = 1; i < 4; i++) {
        if (items[i].x < minX) minX = items[i].x;
        if (items[i].x > maxX) maxX = items[i].x;
    }
    int tetrominoWidth = maxX - minX + 1; // Chiều rộng thực tế của Tetromino
    int offsetX = (GRID_COLS - tetrominoWidth) / 2; // Tính offset để căn giữa

    // Dịch chuyển tất cả các ô của Tetromino
    for (int i = 0; i < 4; i++) {
        items[i].x += offsetX;
    }

    // Đặt lại các giá trị khác
    dx = 0;
    rotate = false;
    delay = 500;
    startTime = SDL_GetTicks();
    currentTime = startTime;
}

void Tetromino::getTetrominoBounds(int& width, int& height, int& minX, int& minY) {
    minX = items[0].x; int maxX = items[0].x;
    minY = items[0].y; int maxY = items[0].y;

    // Tìm tọa độ min và max của Tetromino
    for (int i = 1; i < 4; i++) {
        if (items[i].x < minX) minX = items[i].x;
        if (items[i].x > maxX) maxX = items[i].x;
        if (items[i].y < minY) minY = items[i].y;
        if (items[i].y > maxY) maxY = items[i].y;
    }

    // Tính chiều rộng và chiều cao (số ô)
    width = (maxX - minX + 1) * CELL_SIZE;
    height = (maxY - minY + 1) * CELL_SIZE;
}

void Tetromino::drawBlock(SDL_Renderer* renderer, int x, int y, int cellSize, int colorIndex) {
    SDL_Color colors[7] = {
        { 72, 209, 204, 255 },  // I
        { 106, 90, 205, 255 },  // J
        { 244, 164, 96, 255 },  // L
        { 252, 245, 76, 255 },  // O
        { 173, 255, 47, 255 },  // S
        { 238, 130, 238, 255 }, // T
        { 220, 20, 60, 255 }    // Z
    };

    SDL_Rect block = { x, y, cellSize, cellSize };

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 100);
    SDL_Rect shadowRect = { block.x + 5, block.y + 5, cellSize - 5, cellSize - 5 };
    SDL_RenderFillRect(renderer, &shadowRect);

    SDL_SetRenderDrawColor(renderer,
        colors[colorIndex].r,
        colors[colorIndex].g,
        colors[colorIndex].b,
        255);
    SDL_RenderFillRect(renderer, &block);

    SDL_SetRenderDrawColor(renderer,
        (colors[colorIndex].r + 60 < 255 ? colors[colorIndex].r + 60 : 255),
        (colors[colorIndex].g + 60 < 255 ? colors[colorIndex].g + 60 : 255),
        (colors[colorIndex].b + 60 < 255 ? colors[colorIndex].b + 60 : 255),
        255);
    SDL_RenderDrawLine(renderer, block.x, block.y, block.x + cellSize - 1, block.y);
    SDL_RenderDrawLine(renderer, block.x, block.y, block.x, block.y + cellSize - 1);

    SDL_SetRenderDrawColor(renderer,
        (colors[colorIndex].r - 60 > 0 ? colors[colorIndex].r - 60 : 0),
        (colors[colorIndex].g - 60 > 0 ? colors[colorIndex].g - 60 : 0),
        (colors[colorIndex].b - 60 > 0 ? colors[colorIndex].b - 60 : 0),
        255);
    SDL_RenderDrawLine(renderer, block.x + cellSize - 1, block.y, block.x + cellSize - 1, block.y + cellSize - 1);
    SDL_RenderDrawLine(renderer, block.x, block.y + cellSize - 1, block.x + cellSize - 1, block.y + cellSize - 1);

    SDL_SetRenderDrawColor(renderer,
        (colors[colorIndex].r + 30 < 0 ? colors[colorIndex].r + 30 : 0),
        (colors[colorIndex].g + 30 < 0 ? colors[colorIndex].g + 30 : 0),
        (colors[colorIndex].b + 30 < 0 ? colors[colorIndex].b + 30 : 0),
        255);
    SDL_Rect innerRect = { block.x + 2, block.y + 2, cellSize - 4, cellSize - 4 };
    SDL_RenderDrawRect(renderer, &innerRect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &block);
}

void Tetromino::drawTetromino(SDL_Renderer* renderer, int gridX, int gridY, int cellSize) {
    for (int i = 0; i < 4; i++) {
        int x = gridX + items[i].x * cellSize;
        int y = gridY + items[i].y * cellSize;
        drawBlock(renderer, x, y, cellSize, color - 1);
    }
}

void Tetromino::drawInHold(SDL_Renderer* renderer, int holdX, int holdY, int panelWidth, int panelHeight, int cellSize) {
    // Tính kích thước thực tế và tọa độ nhỏ nhất của Tetromino
    int tetrominoWidth, tetrominoHeight, minX, minY;
    getTetrominoBounds(tetrominoWidth, tetrominoHeight, minX, minY);

    // Căn giữa Tetromino trong khung HOLD
    int offsetX = holdX + (panelWidth - tetrominoWidth) / 2;
    int offsetY = holdY + CELL_SIZE + (panelHeight - tetrominoHeight) / 2; // Dịch xuống dưới label "HOLD"

    // Vẽ từng ô của Tetromino, điều chỉnh tọa độ dựa trên minX và minY
    for (int i = 0; i < 4; i++) {
        int x = offsetX + (items[i].x - minX) * cellSize;
        int y = offsetY + (items[i].y - minY) * cellSize;
        drawBlock(renderer, x, y, cellSize, color - 1);
    }
}

void Tetromino::drawInNext(SDL_Renderer* renderer, int nextX, int nextY, int panelWidth, int panelHeight, int cellSize, int index) {
    // Tính kích thước thực tế và tọa độ nhỏ nhất của Tetromino
    int tetrominoWidth, tetrominoHeight, minX, minY;
    getTetrominoBounds(tetrominoWidth, tetrominoHeight, minX, minY);

    // Căn giữa Tetromino trong khung NEXT
    int offsetX = nextX + (panelWidth - tetrominoWidth) / 2;
    // Mỗi Tetromino chiếm không gian 4 ô theo chiều dọc, cách nhau 10 pixel
    int blockSpaceHeight = 4 * cellSize;
    int offsetY = nextY + CELL_SIZE + index * (blockSpaceHeight - 10) + (blockSpaceHeight - tetrominoHeight) / 2;

    // Vẽ từng ô của Tetromino, điều chỉnh tọa độ dựa trên minX và minY
    for (int i = 0; i < 4; i++) {
        int x = offsetX + (items[i].x - minX) * cellSize;
        int y = offsetY + (items[i].y - minY) * cellSize;
        drawBlock(renderer, x, y, cellSize, color - 1);
    }
}