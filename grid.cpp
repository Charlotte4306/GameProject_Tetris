#include "grid.h"
#include "defs.h"

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

void drawPanel(SDL_Renderer* renderer, int x, int y, int width, int height) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect background = { x, y, width, height };
    SDL_RenderFillRect(renderer, &background);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect border = { x - 2, y - 2, width + 4, height + 4 };
    SDL_RenderDrawRect(renderer, &border);
}

void drawText(SDL_Renderer* renderer, int x, int y, const char* text) {
    
}

void drawUI(SDL_Renderer* renderer) {
    drawPanel(renderer, HOLD_X, HOLD_Y, PANEL_WIDTH, PANEL_HEIGHT);
    drawText(renderer, HOLD_X + 10, HOLD_Y - 20, "HOLD");
    drawPanel(renderer, NEXT_X, NEXT_Y, PANEL_WIDTH, PANEL_HEIGHT);
    drawText(renderer, NEXT_X + 10, NEXT_Y - 20, "NEXT");
}
