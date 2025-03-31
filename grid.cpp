#include "grid.h"
#include <algorithm>
#include <string>


Grid::Grid(Score& scoreRef) : score(scoreRef) {
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            grid[i][j] = 0;
        }
    }
    hasHeld = false;
    nextTetrominos.clear();
}

void Grid::drawGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect gridBackground = { GRID_X, GRID_Y, GRID_WIDTH, GRID_HEIGHT };
    SDL_RenderFillRect(renderer, &gridBackground);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect gridBorder = { GRID_X - 2, GRID_Y - 2, GRID_WIDTH + 4, GRID_HEIGHT + 4 };
    SDL_RenderDrawRect(renderer, &gridBorder);
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    for (int x = 0; x <= GRID_WIDTH; x += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, GRID_X + x, GRID_Y, GRID_X + x, GRID_Y + GRID_HEIGHT);
    }
    for (int y = 0; y <= GRID_HEIGHT; y += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, GRID_X, GRID_Y + y, GRID_X + GRID_WIDTH, GRID_Y + y);
    }
}

void Grid::drawPanel(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int width, int height, const char* text) {
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color black = { 0, 0, 0, 255 };
    SDL_Rect labelBorder = { x - 3, y - 3, width + 6, CELL_SIZE + 6 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &labelBorder);
    SDL_Rect labelRect = { x, y, width, CELL_SIZE };
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
    SDL_RenderFillRect(renderer, &labelRect);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, black);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_x = x + (width - textSurface->w) / 2;
    int text_y = y + (CELL_SIZE - textSurface->h) / 2;
    SDL_Rect textRect = { text_x, text_y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    SDL_Rect panelRect = { x, y + CELL_SIZE, width, height };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &panelRect);
    SDL_Rect border = { x - 3, y + CELL_SIZE - 3, width + 6, height + 6 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &border);
    SDL_Rect innerRect = { x + 1, y + CELL_SIZE + 1, width - 2, height - 2 };
    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    SDL_RenderFillRect(renderer, &innerRect);
}

void Grid::drawUI(SDL_Renderer* renderer, TTF_Font* font) {
    drawPanel(renderer, font, HOLD_X, HOLD_Y, PANEL_WIDTH, PANEL_HEIGHT, "HOLD");
    int nextPanelHeight = PANEL_HEIGHT + 360; 
    drawPanel(renderer, font, NEXT_X, NEXT_Y, PANEL_WIDTH, nextPanelHeight, "NEXT");
    int scorePanelHeight = 3 * CELL_SIZE; 
    int scorePanelY = HOLD_Y + PANEL_HEIGHT + 20; 
    drawPanel(renderer, font, HOLD_X, scorePanelY, PANEL_WIDTH, scorePanelHeight, "SCORE");
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color black = { 0, 0, 0, 255 };
    std::string scoreText = "Score: " + std::to_string(score.score);
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    int scoreTextX = HOLD_X + (PANEL_WIDTH - scoreSurface->w) / 2;
    int scoreTextY = scorePanelY + CELL_SIZE + 20; 
    SDL_Rect scoreRect = { scoreTextX, scoreTextY, scoreSurface->w, scoreSurface->h };
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
    std::string levelText = "Level: " + std::to_string(score.level);
    SDL_Surface* levelSurface = TTF_RenderText_Solid(font, levelText.c_str(), white);
    SDL_Texture* levelTexture = SDL_CreateTextureFromSurface(renderer, levelSurface);
    int levelTextX = HOLD_X + (PANEL_WIDTH - levelSurface->w) / 2;
    int levelTextY = scorePanelY + CELL_SIZE + 80; 
    SDL_Rect levelRect = { levelTextX, levelTextY, levelSurface->w, levelSurface->h };
    SDL_RenderCopy(renderer, levelTexture, NULL, &levelRect);
    SDL_FreeSurface(levelSurface);
    SDL_DestroyTexture(levelTexture);
    if (hasHeld) {
        heldTetromino.drawInHold(renderer, HOLD_X, HOLD_Y, PANEL_WIDTH, PANEL_HEIGHT, CELL_SIZE);
    }
    for (size_t i = 0; i < nextTetrominos.size(); i++) {
        nextTetrominos[i].drawInNext(renderer, NEXT_X, NEXT_Y, PANEL_WIDTH, nextPanelHeight, CELL_SIZE, i);
    }
}

void Grid::drawTetrimino(SDL_Renderer* renderer, Tetromino& tetromino) {
    tetromino.drawTetromino(renderer, GRID_X, GRID_Y, CELL_SIZE);
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            if (grid[i][j]) {
                int x = GRID_X + j * CELL_SIZE;
                int y = GRID_Y + i * CELL_SIZE;
                Tetromino::drawBlock(renderer, x, y, CELL_SIZE, grid[i][j] - 1);
            }
        }
    }
}