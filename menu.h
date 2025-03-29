#ifndef _MENU_H
#define _MENU_H
#include <SDL.h>
#include <SDL_ttf.h>
#include "graphics.h"


struct Menu {
    Graphics* graphics; 
    SDL_Texture* background; 
    SDL_Texture* logoTexture;
    TTF_Font* titleFont; 
    TTF_Font* buttonFont; 
    SDL_Rect playButton; 
    SDL_Rect settingButton; 
    SDL_Rect helpButton; 
    bool playClicked; 
    bool settingsClicked; 
    bool helpClicked; 
    void initMenu(Menu*, Graphics*, TTF_Font*, TTF_Font*, SDL_Texture*);
    void drawMenu(Menu*);
    void handleMenuEvents(Menu*, SDL_Event*);
    void freeMenu(Menu*);
};

#endif _MENU_H