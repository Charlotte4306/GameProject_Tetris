#include "menu.h"

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

void Menu::initGameOverMenu(Menu* menu, Graphics* graphics, TTF_Font* titleFont, TTF_Font* buttonFont, SDL_Texture* background) {
    menu->graphics = graphics;
    menu->background = background;
    menu->titleFont = titleFont;
    menu->buttonFont = buttonFont;
    menu->playAgainButton = { 500, 500, 200, 60 }; 
    menu->quitButton = { 750, 500, 200, 60 };      
    menu->playAgainClicked = false;
    menu->quitClicked = false;
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
void Menu::handleGameOverEvents(Menu* menu, SDL_Event* e) {
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
        case SDLK_r:
            menu->playAgainClicked = true;
            break;
        case SDLK_ESCAPE:
            menu->quitClicked = true;
            break;
        }
    }
}
void Menu::drawGameOver(Menu* menu, Score& score) {
    menu->graphics->prepareScene(menu->background);

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color black = { 0, 0, 0, 255 };

    // Vẽ tiêu đề "GAME OVER"
    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(menu->titleFont, "GAME OVER", white);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(menu->graphics->renderer, gameOverSurface);
    SDL_Rect gameOverRect = { 500, 200, gameOverSurface->w, gameOverSurface->h };
    SDL_RenderCopy(menu->graphics->renderer, gameOverTexture, NULL, &gameOverRect);
    SDL_FreeSurface(gameOverSurface);
    SDL_DestroyTexture(gameOverTexture);

    // Vẽ điểm số
    std::string scoreText = "Score: " + std::to_string(score.score);
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(menu->buttonFont, scoreText.c_str(), white);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(menu->graphics->renderer, scoreSurface);
    SDL_Rect scoreRect = { 500, 300, scoreSurface->w, scoreSurface->h };
    SDL_RenderCopy(menu->graphics->renderer, scoreTexture, NULL, &scoreRect);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);

    // Vẽ level
    std::string levelText = "Level: " + std::to_string(score.level);
    SDL_Surface* levelSurface = TTF_RenderText_Solid(menu->buttonFont, levelText.c_str(), white);
    SDL_Texture* levelTexture = SDL_CreateTextureFromSurface(menu->graphics->renderer, levelSurface);
    SDL_Rect levelRect = { 500, 350, levelSurface->w, levelSurface->h };
    SDL_RenderCopy(menu->graphics->renderer, levelTexture, NULL, &levelRect);
    SDL_FreeSurface(levelSurface);
    SDL_DestroyTexture(levelTexture);

    // Vẽ hướng dẫn phím
    SDL_Surface* playAgainSurface = TTF_RenderText_Solid(menu->buttonFont, "Press R to Play Again", white);
    SDL_Texture* playAgainTexture = SDL_CreateTextureFromSurface(menu->graphics->renderer, playAgainSurface);
    SDL_Rect playAgainRect = { 500, 450, playAgainSurface->w, playAgainSurface->h };
    SDL_RenderCopy(menu->graphics->renderer, playAgainTexture, NULL, &playAgainRect);
    SDL_FreeSurface(playAgainSurface);
    SDL_DestroyTexture(playAgainTexture);

    SDL_Surface* quitSurface = TTF_RenderText_Solid(menu->buttonFont, "Press ESC to Quit", white);
    SDL_Texture* quitTexture = SDL_CreateTextureFromSurface(menu->graphics->renderer, quitSurface);
    SDL_Rect quitRect = { 500, 500, quitSurface->w, quitSurface->h };
    SDL_RenderCopy(menu->graphics->renderer, quitTexture, NULL, &quitRect);
    SDL_FreeSurface(quitSurface);
    SDL_DestroyTexture(quitTexture);

    menu->graphics->presentScene();
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