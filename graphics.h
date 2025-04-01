#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <SDL_image.h>
struct Graphics {
    SDL_Renderer* renderer;
    SDL_Window* window;
    TTF_Font* font;
    Mix_Music* menuBackgroundMusic; // Nhạc cố định cho menu
    Mix_Music* currentBackgroundMusic; // Nhạc hiện tại cho phần chơi
    std::vector<Mix_Music*> backgroundMusicList; // Danh sách nhạc cho phần chơi

    void logErrorAndExit(const char*, const char*);
    SDL_Texture* loadTexture(const char*);
    TTF_Font* loadFont(const char*, int);
    SDL_Texture* renderText(const char*, TTF_Font*, SDL_Color);
    void init();
    void presentScene();
    void prepareScene(SDL_Texture*);
    void blitRect(SDL_Texture*, SDL_Rect*, int, int);
    void renderTexture(SDL_Texture*, int, int);
    void quit();
    void loadMenuBackgroundMusic(); // Tải nhạc cố định cho menu
    void loadBackgroundMusicList(); // Tải danh sách nhạc cho phần chơi
    void playMenuBackgroundMusic(); // Phát nhạc menu
    void playBackgroundMusic(int index); // Phát nhạc theo chỉ số
    void stopBackgroundMusic(); // Dừng nhạc
};

#endif _GRAPHICS_H