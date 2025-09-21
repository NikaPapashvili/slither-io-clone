#pragma once

#include "Vector2.h"
#include <SDL2/SDL.h>
#include <random>

class Food {
public:
    Vector2 position;
    float radius;
    SDL_Color color;
    bool active;
    int value;
    
    // Types de nourriture
    enum Type {
        NORMAL = 0,
        BIG = 1,
        GOLDEN = 2
    };
    
    Type type;

    Food();
    ~Food() = default;

    void spawn(float x, float y, Type foodType = NORMAL);
    void reset();
    void render(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Vector2& playerPos);
    
    // Utilitaires
    bool isColliding(const Vector2& playerPos, float playerRadius) const;
    void setRandomColor();
    void setRandomPosition(int screenWidth, int screenHeight, float mapRadius);
    
private:
    std::mt19937 rng;
    std::uniform_real_distribution<float> colorDist;
    std::uniform_real_distribution<float> positionDist;
};
