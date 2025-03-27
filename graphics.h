#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <SDL.h>
#include <SDL_ttf.h>
struct Graphics {
	SDL_Renderer* renderer;
	SDL_Window* window;
	TTF_Font* font;
	void logErrorAndExit(const char*, const char*);
	SDL_Texture* loadTexture(const char*);
	TTF_Font* loadFont(const char*, int);
	SDL_Texture* renderText(const char*, TTF_Font*, SDL_Color );
	void init();
	//void prepareScene();
	void presentScene();
	void prepareScene(SDL_Texture*);
	void blitRect(SDL_Texture*, SDL_Rect*, int, int);
	void renderTexture(SDL_Texture* , int, int);
	void quit();
};

#endif _GRAPHICS_H