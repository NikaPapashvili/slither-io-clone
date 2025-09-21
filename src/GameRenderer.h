#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class GameRenderer {
public:
    static GameRenderer& getInstance();
    
    bool initialize(int width, int height);
    void cleanup();
    
    void beginFrame();
    void endFrame();
    
    void renderMap(int screenWidth, int screenHeight, float mapRadius);
    void renderUI(int score, bool alive);
    
    SDL_Renderer* getRenderer() const { return renderer; }
    SDL_Window* getWindow() const { return window; }
    
private:
    GameRenderer() = default;
    ~GameRenderer() = default;
    GameRenderer(const GameRenderer&) = delete;
    GameRenderer& operator=(const GameRenderer&) = delete;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    
    int screenWidth;
    int screenHeight;
    
    // Couleurs
    SDL_Color backgroundColor;
    SDL_Color mapColor;
    SDL_Color textColor;
    SDL_Color gameOverColor;
    
    void drawCircle(int centerX, int centerY, int radius, SDL_Color color, bool filled = true);
    void drawText(const std::string& text, int x, int y, SDL_Color color, int fontSize = 24);
};
