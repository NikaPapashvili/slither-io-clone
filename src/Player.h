#pragma once

#include "Vector2.h"
#include <vector>
#include <SDL2/SDL.h>

class Player {
public:
    Vector2 position;
    Vector2 velocity;
    Vector2 target;
    float speed;
    float radius;
    int score;
    std::vector<Vector2> body;
    SDL_Color color;
    bool alive;

    Player(float x, float y);
    ~Player() = default;

    void update(float deltaTime);
    void setTarget(float x, float y);
    void grow();
    void reset();
    void render(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    
    // Collision detection
    bool checkSelfCollision() const;
    bool checkWallCollision(int screenWidth, int screenHeight, float mapRadius) const;
    bool checkFoodCollision(const Vector2& foodPos, float foodRadius) const;
    
    // Utilitaires
    float getBodyRadius() const;
    Vector2 getHeadPosition() const;
    void updateBody();
    void limitToMap(int screenWidth, int screenHeight, float mapRadius);
};
