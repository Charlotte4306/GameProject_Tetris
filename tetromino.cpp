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
    // Nếu danh sách nextTetrominos rỗng, tạo mới 4 khối
    if (nextTetrominos.empty()) {
        for (int i = 0; i < 4; i++) {
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

    // Lấy khối đầu tiên từ danh sách nextTetrominos
    *this = nextTetrominos.front();
    nextTetrominos.erase(nextTetrominos.begin());

    // Thêm một khối mới vào cuối danh sách
    Tetromino next;
    next.type = rand() % 7;
    next.color = next.type + 1;
    for (int j = 0; j < 4; j++) {
        next.items[j].x = figures[next.type][j] % 4;
        next.items[j].y = figures[next.type][j] / 4;
    }
    nextTetrominos.push_back(next);
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
    // Tính toán để căn giữa khối trong khung HOLD
    int blockWidth = 4 * cellSize; // Chiều rộng tối đa của Tetromino (4 ô)
    int blockHeight = 4 * cellSize; // Chiều cao tối đa của Tetromino (4 ô)
    int offsetX = holdX + (panelWidth - blockWidth) / 2;
    int offsetY = holdY + (panelHeight - blockHeight) / 2 + CELL_SIZE; // Cộng thêm CELL_SIZE để dưới nhãn "HOLD"

    // Điều chỉnh vị trí để khối không lệch ra ngoài
    for (int i = 0; i < 4; i++) {
        int x = offsetX + (items[i].x - 1) * cellSize; // Dịch trái 1 ô để căn giữa
        int y = offsetY + items[i].y * cellSize;
        drawBlock(renderer, x, y, cellSize, color - 1);
    }
}

void Tetromino::drawInNext(SDL_Renderer* renderer, int nextX, int nextY, int panelWidth, int panelHeight, int cellSize, int index) {
    // Tính toán vị trí để căn giữa và xếp các khối theo chiều dọc trong khung NEXT
    int blockWidth = 4 * cellSize;
    int blockHeight = 4 * cellSize;
    int offsetX = nextX + (panelWidth - blockWidth) / 2;
    int offsetY = nextY + CELL_SIZE + index * (blockHeight + 10); // Mỗi khối cách nhau 10px, cộng CELL_SIZE để dưới nhãn "NEXT"

    // Điều chỉnh vị trí để khối không lệch ra ngoài
    for (int i = 0; i < 4; i++) {
        int x = offsetX + (items[i].x - 1) * cellSize; // Dịch trái 1 ô để căn giữa
        int y = offsetY + items[i].y * cellSize;
        drawBlock(renderer, x, y, cellSize, color - 1);
    }
}