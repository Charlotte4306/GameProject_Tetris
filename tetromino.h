#ifndef TETROMINO_H_
#define TETROMINO_H_
#include <SDL.h>
#include "defs.h"
#include <vector> // Để dùng std::vector lưu danh sách nextTetrominos

struct Point {
    int x, y;
};

struct Tetromino {
    Point items[4], backup[4];
    int color;
    int type;
    int dx;
    bool rotate;
    unsigned int delay;
    Uint32 startTime, currentTime;
    static const int figures[7][4];

    void initTetromino();
    void nextTetromino(std::vector<Tetromino>& nextTetrominos); // Thêm tham số để quản lý danh sách next
    void drawTetromino(SDL_Renderer* renderer, int gridX, int gridY, int cellSize);
    void drawInHold(SDL_Renderer* renderer, int holdX, int holdY, int panelWidth, int panelHeight, int cellSize); // Vẽ trong khung HOLD
    void drawInNext(SDL_Renderer* renderer, int nextX, int nextY, int panelWidth, int panelHeight, int cellSize, int index); // Vẽ trong khung NEXT
    static void drawBlock(SDL_Renderer* renderer, int x, int y, int cellSize, int colorIndex);
};

#endif // TETROMINO_H_