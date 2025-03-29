#include "menu.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
void Menu::initMenu(Menu* menu, Graphics* graphics, TTF_Font* titleFont, TTF_Font* buttonFont, SDL_Texture* background) {
    menu->graphics = graphics;
    menu->background = background;
    menu->titleFont = titleFont;
    menu->buttonFont = buttonFont;
    menu->playClicked = false;
    menu->settingsClicked = false;
    menu->helpClicked = false;

    int buttonWidth = 400;
    int buttonHeight = 80;
    int buttonSpacing = 40;

    menu->logoTexture = graphics->loadTexture("Background/LOGO1.JPG"); 
    if (menu->logoTexture == nullptr) {
        SDL_Log("Failed to load Tetris logo: %s", IMG_GetError());
    }

    menu->playButton = { 0, 0, buttonWidth, buttonHeight };
    menu->settingButton = { 0, 0, buttonWidth, buttonHeight };
    menu->helpButton = { 0, 0, buttonWidth, buttonHeight };
}

void Menu::drawMenu(Menu* menu) {
    menu->graphics->prepareScene(menu->background);
    SDL_SetRenderDrawBlendMode(menu->graphics->renderer, SDL_BLENDMODE_BLEND); 
    SDL_SetRenderDrawColor(menu->graphics->renderer, 0, 0, 0, 150);
    SDL_Rect gridBackground = { GRID_X, GRID_Y, GRID_WIDTH, GRID_HEIGHT };
    SDL_RenderFillRect(menu->graphics->renderer, &gridBackground);
    SDL_SetRenderDrawColor(menu->graphics->renderer, 255, 255, 255, 255);
    SDL_Rect gridBorder = { GRID_X - 3, GRID_Y - 3, GRID_WIDTH + 6, GRID_HEIGHT + 6 };
    SDL_RenderDrawRect(menu->graphics->renderer, &gridBorder);
    SDL_SetRenderDrawColor(menu->graphics->renderer, 0, 255, 255, 255);
    int borderThickness = 5; 
    for (int i = 0; i < borderThickness; i++) {
        SDL_Rect gridBorder = { GRID_X - i, GRID_Y - i, GRID_WIDTH + 2 * i, GRID_HEIGHT + 2 * i };
        SDL_RenderDrawRect(menu->graphics->renderer, &gridBorder);
    }

    if (menu->logoTexture != nullptr) {
        int logoWidth, logoHeight;
        SDL_QueryTexture(menu->logoTexture, NULL, NULL, &logoWidth, &logoHeight);
        float scaleFactor = 2.0f; 
        int scaledLogoWidth = static_cast<int>(logoWidth * scaleFactor);
        int scaledLogoHeight = static_cast<int>(logoHeight * scaleFactor);
        int logoX = GRID_X + (GRID_WIDTH - scaledLogoWidth) / 2; 
        int logoY = GRID_Y + 20; 
        SDL_Rect logoRect = { logoX, logoY, scaledLogoWidth, scaledLogoHeight };
        SDL_RenderCopy(menu->graphics->renderer, menu->logoTexture, nullptr, &logoRect);
        int buttonYStart = logoY + scaledLogoHeight + 40; 
        menu->playButton.x = GRID_X + (GRID_WIDTH - menu->playButton.w) / 2; 
        menu->playButton.y = buttonYStart;
        menu->settingButton.x = GRID_X + (GRID_WIDTH - menu->settingButton.w) / 2;
        menu->settingButton.y = buttonYStart + menu->playButton.h + 40; 
        menu->helpButton.x = GRID_X + (GRID_WIDTH - menu->helpButton.w) / 2;
        menu->helpButton.y = buttonYStart + 2 * (menu->playButton.h + 40);
    }

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color darkBlue = { 0, 0, 50, 255 };
    SDL_Color neonBlue = { 0, 255, 255, 255 };

    SDL_SetRenderDrawColor(menu->graphics->renderer, darkBlue.r, darkBlue.g, darkBlue.b, darkBlue.a);
    SDL_RenderFillRect(menu->graphics->renderer, &menu->playButton);
    SDL_SetRenderDrawColor(menu->graphics->renderer, neonBlue.r, neonBlue.g, neonBlue.b, neonBlue.a);
    SDL_RenderDrawRect(menu->graphics->renderer, &menu->playButton);
    SDL_Texture* playText = menu->graphics->renderText("PLAY", menu->buttonFont, white);
    menu->graphics->renderTexture(playText, menu->playButton.x + (menu->playButton.w - 100) / 2, menu->playButton.y + (menu->playButton.h - 40) / 2);
    SDL_DestroyTexture(playText);

    SDL_SetRenderDrawColor(menu->graphics->renderer, darkBlue.r, darkBlue.g, darkBlue.b, darkBlue.a);
    SDL_RenderFillRect(menu->graphics->renderer, &menu->settingButton);
    SDL_SetRenderDrawColor(menu->graphics->renderer, neonBlue.r, neonBlue.g, neonBlue.b, neonBlue.a);
    SDL_RenderDrawRect(menu->graphics->renderer, &menu->settingButton);
    SDL_Texture* settingText = menu->graphics->renderText("SETTING", menu->buttonFont, white);
    menu->graphics->renderTexture(settingText, menu->settingButton.x + (menu->settingButton.w - 180) / 2, menu->settingButton.y + (menu->settingButton.h - 40) / 2);
    SDL_DestroyTexture(settingText);

    SDL_SetRenderDrawColor(menu->graphics->renderer, darkBlue.r, darkBlue.g, darkBlue.b, darkBlue.a);
    SDL_RenderFillRect(menu->graphics->renderer, &menu->helpButton);
    SDL_SetRenderDrawColor(menu->graphics->renderer, neonBlue.r, neonBlue.g, neonBlue.b, neonBlue.a);
    SDL_RenderDrawRect(menu->graphics->renderer, &menu->helpButton);
    SDL_Texture* helpText = menu->graphics->renderText("HELP", menu->buttonFont, white);
    menu->graphics->renderTexture(helpText, menu->helpButton.x + (menu->helpButton.w - 100) / 2, menu->helpButton.y + (menu->helpButton.h - 40) / 2);
    SDL_DestroyTexture(helpText);

    menu->graphics->presentScene();
}

void Menu::handleMenuEvents(Menu* menu, SDL_Event* e) {
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= menu->playButton.x && x <= menu->playButton.x + menu->playButton.w &&
            y >= menu->playButton.y && y <= menu->playButton.y + menu->playButton.h) {
            menu->playClicked = true;
        }
        if (x >= menu->settingButton.x && x <= menu->settingButton.x + menu->settingButton.w &&
            y >= menu->settingButton.y && y <= menu->settingButton.y + menu->settingButton.h) {
            menu->settingsClicked = true;
        }
        if (x >= menu->helpButton.x && x <= menu->helpButton.x + menu->helpButton.w &&
            y >= menu->helpButton.y && y <= menu->helpButton.y + menu->helpButton.h) {
            menu->helpClicked = true;
        }
    }
}

void Menu::freeMenu(Menu* menu) {
    if (menu->logoTexture != nullptr) {
        SDL_DestroyTexture(menu->logoTexture);
        menu->logoTexture = nullptr;
    }
    menu->graphics = nullptr;
    menu->background = nullptr;
    menu->titleFont = nullptr;
    menu->buttonFont = nullptr;
}