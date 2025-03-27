#include "Tetromino.h"
#include <iostream>
const int Tetrominoes[7][TETROMINO_SIZE][TETROMINO_SIZE] = {
    {
        { 0, 0, 0, 0 },
        { 1, 1, 1, 1 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 1, 1, 0, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 1, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 1, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 0, 1, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 }
    }
};

void drawTetromino(SDL_Renderer* renderer, int tetrominoType, int startX, int startY, int cellSize) {
    SDL_Color colors[7] = {
        { 72, 209, 204, 255 },       // I       //  ||         ||  
        { 252, 245, 76, 255 },      //  O      //   ||         ||
        { 238, 130, 238, 255 },    //   T     //    ||_________||
        { 173, 255, 47, 255 },    //    S    //     ||_________||
        { 220, 20, 60, 255 },    //     Z   //      ||         ||
        { 106, 90, 205, 255 },  //      J  //       ||         ||
        { 244, 164, 96, 255 }  //       L //        ||         ||
    };
    for (int row = 0; row < TETROMINO_SIZE; row++) {
        for (int col = 0; col < TETROMINO_SIZE; col++) {
            if (Tetrominoes[tetrominoType][row][col]) {
                SDL_Rect block = { startX + col * cellSize, startY + row * cellSize, cellSize, cellSize };
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 100);
                SDL_Rect shadowRect = { block.x + 5, block.y + 5, cellSize - 5, cellSize - 5 };
                SDL_RenderFillRect(renderer, &shadowRect);
                SDL_SetRenderDrawColor(renderer, colors[tetrominoType].r, colors[tetrominoType].g, colors[tetrominoType].b, 255);
                SDL_RenderFillRect(renderer, &block);
                SDL_SetRenderDrawColor(renderer,
                    std::min(255, colors[tetrominoType].r + 60),
                    std::min(255, colors[tetrominoType].g + 60),
                    std::min(255, colors[tetrominoType].b + 60),
                    255);
                SDL_RenderDrawLine(renderer, block.x, block.y, block.x + cellSize - 1, block.y); 
                SDL_RenderDrawLine(renderer, block.x, block.y, block.x, block.y + cellSize - 1); 
                SDL_SetRenderDrawColor(renderer,
                    std::max(0, colors[tetrominoType].r - 60),
                    std::max(0, colors[tetrominoType].g - 60),
                    std::max(0, colors[tetrominoType].b - 60),
                    255);
                SDL_RenderDrawLine(renderer, block.x + cellSize - 1, block.y, block.x + cellSize - 1, block.y + cellSize - 1); 
                SDL_RenderDrawLine(renderer, block.x, block.y + cellSize - 1, block.x + cellSize - 1, block.y + cellSize - 1); 
                SDL_SetRenderDrawColor(renderer,
                    std::min(0, colors[tetrominoType].r + 30),
                    std::min(0, colors[tetrominoType].g + 30),
                    std::min(0, colors[tetrominoType].b + 30),
                    255);
                SDL_Rect innerRect = { block.x + 2, block.y + 2, cellSize - 4, cellSize - 4 };
                SDL_RenderDrawRect(renderer, &innerRect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &block);
            }
        }
    }
}


