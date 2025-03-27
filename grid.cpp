#include "grid.h"
#include "defs.h"
#include <SDL_ttf.h>
void drawGrid(SDL_Renderer* renderer) {
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

void drawPanel(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int width, int height, const char* text) {
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

//void drawText(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int width, const char* text, SDL_Color color) {
//    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
//    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//
//    // Đặt chữ gần mép trên nhưng vẫn trong panel (ví dụ: cách 5px từ mép trên)
//    int text_x = x + (width - surface->w) / 2;
//    int text_y = y + 5; // Đẩy chữ lên sát mép trên
//
//    SDL_Rect textRect = { text_x, text_y, surface->w, surface->h };
//    SDL_RenderCopy(renderer, texture, NULL, &textRect);
//
//    SDL_FreeSurface(surface);
//    SDL_DestroyTexture(texture);
//}

void drawUI(SDL_Renderer* renderer, TTF_Font* font) {
    drawPanel(renderer, font, HOLD_X, HOLD_Y, PANEL_WIDTH, PANEL_HEIGHT, "HOLD");
    int nextPanelHeight = PANEL_HEIGHT + 360;
    drawPanel(renderer, font, NEXT_X, NEXT_Y, PANEL_WIDTH, nextPanelHeight, "NEXT");
}
