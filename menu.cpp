//#include "menu.h"
//#include <SDL.h>
//#include <SDL_ttf.h>
//#include "defs.h"
//void Menu::drawGrid(SDL_Renderer* renderer) {
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
//    for (int i = 0; i <= GRID_COLS; i++) {
//        SDL_RenderDrawLine(renderer, GRID_X + i * CELL_SIZE, GRID_Y,
//            GRID_X + i * CELL_SIZE, GRID_Y + GRID_ROWS * CELL_SIZE);
//    }
//    for (int j = 0; j <= GRID_ROWS; j++) {
//        SDL_RenderDrawLine(renderer, GRID_X, GRID_Y + j * CELL_SIZE,
//            GRID_X + GRID_COLS * CELL_SIZE, GRID_Y + j * CELL_SIZE);
//    }
//}
//
//// Hàm vẽ nút bấm
//void Menu::drawButton(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int w, int h, const char* text) {
//    SDL_Rect button = { x, y, w, h };
//    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); 
//    SDL_RenderFillRect(renderer, &button);
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//    SDL_RenderDrawRect(renderer, &button);
//
//    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, { 255, 255, 255 });
//    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
//    SDL_Rect textRect = { x + (w - textSurface->w) / 2, y + (h - textSurface->h) / 2, textSurface->w, textSurface->h };
//    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
//
//    SDL_FreeSurface(textSurface);
//    SDL_DestroyTexture(textTexture);
//}
//
//// Vẽ menu chính (các nút điều khiển)
//void Menu::drawMenu(SDL_Renderer* renderer, TTF_Font* font) {
//    int menuX = 800; 
//    int menuY = 200;
//
//    drawButton(renderer, font, menuX, menuY, 200, 60, "PLAY");
//    drawButton(renderer, font, menuX, menuY + 80, 200, 60, "SETTINGS");
//    drawButton(renderer, font, menuX, menuY + 160, 200, 60, "HELP");
//}