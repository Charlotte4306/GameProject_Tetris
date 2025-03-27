#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "grid.h"
//#include "defs.h"

int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();
    SDL_Texture* background = graphics.loadTexture("Background/PIC2.JPG");
    
    TTF_Font* font = graphics.loadFont("Fonts/Disney.ttf", 24);
    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            break;

        graphics.prepareScene(background);
        drawGrid(graphics.renderer); 
        drawUI(graphics.renderer); 
        graphics.presentScene();
        SDL_Delay(100);
    }
    graphics.quit();
	return 0;
}