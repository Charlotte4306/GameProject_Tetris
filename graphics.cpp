﻿#include "graphics.h"
#include "defs.h"
#include <string>
void Graphics::logErrorAndExit(const char* msg, const char* error) {
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
	SDL_Quit();
}
SDL_Texture* Graphics::loadTexture(const char* filename) {
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
	SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
	if (texture == NULL)
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());
	return texture;
}
void Graphics::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		logErrorAndExit("SDL_Init", SDL_GetError());
	window = SDL_CreateWindow("TETRIS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (window == nullptr)
		logErrorAndExit("CreateWindow", SDL_GetError());
	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
		logErrorAndExit("SDL_image error:", IMG_GetError());
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		logErrorAndExit("SDL_mixer could not initialize! SDL_mixeer Error: %s\n", Mix_GetError());
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
		logErrorAndExit("CreateRenderer", SDL_GetError());
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	//SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (TTF_Init() == -1) {
		logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ", TTF_GetError());
	}
	menuBackgroundMusic = nullptr;
	currentBackgroundMusic = nullptr;
}
//void Graphics::prepareScene() {
//	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//	SDL_RenderClear(renderer);
//}
void Graphics::presentScene() {
	SDL_RenderPresent(renderer);
}
void Graphics::prepareScene(SDL_Texture* background) {
	SDL_RenderClear(renderer);
	if (background != nullptr)
		SDL_RenderCopy(renderer, background, NULL, NULL);
}
void Graphics::renderTexture(SDL_Texture* texture, int x, int y) {
	if (texture == NULL)
		return;
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}
void Graphics::quit() {
	IMG_Quit();
	Mix_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}
TTF_Font* Graphics::loadFont(const char* path, int size)
{
	TTF_Font* gFont = TTF_OpenFont(path, size);
	if (gFont == nullptr) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
	}
	return gFont;
}
SDL_Texture* Graphics::renderText(const char* text, TTF_Font* font, SDL_Color textColor)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
	if (textSurface == nullptr) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Render text surface %s", TTF_GetError());
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (texture == nullptr) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Create texture from text %s", SDL_GetError());
	}

	SDL_FreeSurface(textSurface);
	return texture;
}

void Graphics::loadMenuBackgroundMusic() {
	menuBackgroundMusic = Mix_LoadMUS("Sound/BGM.mp3");
	if (menuBackgroundMusic == nullptr) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
			"Failed to load menu background music: %s", Mix_GetError());
	}
	else {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
			"Successfully loaded menu background music");
	}
}

void Graphics::loadBackgroundMusicList() {
	for (int i = 1; i <= 10; i++) {
		std::string filePath = "Sound/Sound" + std::to_string(i) + ".mp3";
		Mix_Music* music = Mix_LoadMUS(filePath.c_str());
		if (music == nullptr) {
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
				"Failed to load background music %s: %s", filePath.c_str(), Mix_GetError());
		}
		else {
			backgroundMusicList.push_back(music);
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
				"Successfully loaded background music: %s", filePath.c_str());
		}
	}
}

void Graphics::playMenuBackgroundMusic() {
	stopBackgroundMusic();
	if (menuBackgroundMusic != nullptr) {
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		if (Mix_PlayMusic(menuBackgroundMusic, -1) == -1) {
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
				"Failed to play menu background music: %s", Mix_GetError());
		}
		else {
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
				"Playing menu background music...");
		}
	}
}

void Graphics::playBackgroundMusic(int index) {
	if (index >= 0 && index < backgroundMusicList.size()) {
		stopBackgroundMusic();
		currentBackgroundMusic = backgroundMusicList[index];
		if (currentBackgroundMusic != nullptr) {
			Mix_VolumeMusic(MIX_MAX_VOLUME);
			if (Mix_PlayMusic(currentBackgroundMusic, -1) == -1) {
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
					"Failed to play background music: %s", Mix_GetError());
			}
			else {
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
					"Playing background music %d...", index + 1);
			}
		}
	}
	else {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
			"Invalid background music index: %d", index);
	}
}

void Graphics::stopBackgroundMusic() {
	Mix_HaltMusic();
}