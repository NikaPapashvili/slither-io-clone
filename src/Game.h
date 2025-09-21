#pragma once

#include "Player.h"
#include "Food.h"
#include <vector>
#include <memory>
#include <random>

class Game {
public:
    Game(int width, int height);
    ~Game() = default;

    bool initialize();
    void run();
    void cleanup();

private:
    // Dimensions de l'écran
    int screenWidth;
    int screenHeight;
    float mapRadius;
    
    // Joueur
    std::unique_ptr<Player> player;
    
    // Nourriture
    std::vector<Food> foods;
    static const int MAX_FOOD = 50;
    
    // État du jeu
    bool running;
    bool paused;
    float lastTime;
    float deltaTime;
    
    // Générateur aléatoire
    std::mt19937 rng;
    std::uniform_real_distribution<float> angleDist;
    std::uniform_real_distribution<float> distanceDist;
    std::uniform_int_distribution<int> foodTypeDist;
    
    // Méthodes principales
    void update(float deltaTime);
    void render();
    void handleEvents();
    
    // Gestion de la nourriture
    void spawnFood();
    void updateFood();
    void checkFoodCollisions();
    
    // Gestion des collisions
    void checkCollisions();
    
    // Utilitaires
    Vector2 getRandomMapPosition();
    bool isPositionValid(const Vector2& pos, float radius);
    void resetGame();
};
