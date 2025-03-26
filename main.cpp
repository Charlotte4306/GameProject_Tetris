#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
//#include "defs.h"
using namespace std;
void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e) != 0 &&
            (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(1000);
    }
}
int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();
    SDL_Texture* background = graphics.loadTexture("Background/PIC1.JPG");
    graphics.prepareScene(background);
    graphics.presentScene();
    
    waitUntilKeyPressed();
    // Vẽ khu vực game (lưới)
    /*graphics.drawGrid(graphics.renderer);*/
    graphics.quit();
	return 0;
}