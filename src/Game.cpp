#include "Game.h"
#include "GameRenderer.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <cmath>

Game::Game(int width, int height) 
    : screenWidth(width), screenHeight(height), 
      mapRadius(std::min(width, height) * 0.4f),
      running(false), paused(false), lastTime(0), deltaTime(0),
      rng(std::random_device{}()),
      angleDist(0.0f, 2.0f * M_PI),
      distanceDist(50.0f, mapRadius - 100.0f),
      foodTypeDist(0, 2) {
    
    player = std::make_unique<Player>(width / 2, height / 2);
    foods.reserve(MAX_FOOD);
}

bool Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erreur SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if (!GameRenderer::getInstance().initialize(screenWidth, screenHeight)) {
        std::cerr << "Erreur initialisation du rendu" << std::endl;
        return false;
    }
    
    // Initialiser la nourriture
    for (int i = 0; i < MAX_FOOD; ++i) {
        foods.emplace_back();
        spawnFood();
    }
    
    running = true;
    lastTime = SDL_GetTicks() / 1000.0f;
    
    return true;
}

void Game::run() {
    while (running) {
        float currentTime = SDL_GetTicks() / 1000.0f;
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Limiter le deltaTime pour éviter les gros sauts
        deltaTime = std::min(deltaTime, 0.016f); // Max 60 FPS
        
        handleEvents();
        
        if (!paused) {
            update(deltaTime);
        }
        
        render();
        
        // Limiter à 60 FPS
        SDL_Delay(16);
    }
}

void Game::cleanup() {
    GameRenderer::getInstance().cleanup();
    SDL_Quit();
}

void Game::update(float deltaTime) {
    // Mettre à jour le joueur
    player->update(deltaTime);
    
    // Limiter le joueur à la map
    player->limitToMap(screenWidth, screenHeight, mapRadius);
    
    // Vérifier les collisions
    checkCollisions();
    
    // Mettre à jour la nourriture
    updateFood();
    
    // Faire apparaître de la nouvelle nourriture si nécessaire
    int activeFoodCount = 0;
    for (const auto& food : foods) {
        if (food.active) activeFoodCount++;
    }
    
    if (activeFoodCount < MAX_FOOD * 0.8f) {
        spawnFood();
    }
}

void Game::render() {
    GameRenderer::getInstance().beginFrame();
    
    // Rendre la map (cercle)
    GameRenderer::getInstance().renderMap(screenWidth, screenHeight, mapRadius);
    
    // Rendre la nourriture
    for (auto& food : foods) {
        food.render(GameRenderer::getInstance().getRenderer(), 
                   screenWidth, screenHeight, player->getHeadPosition());
    }
    
    // Rendre le joueur
    player->render(GameRenderer::getInstance().getRenderer(), 
                  screenWidth, screenHeight);
    
    // Rendre l'UI
    GameRenderer::getInstance().renderUI(player->score, player->alive);
    
    GameRenderer::getInstance().endFrame();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_p:
                        paused = !paused;
                        break;
                    case SDLK_r:
                        resetGame();
                        break;
                }
                break;
                
            case SDL_MOUSEMOTION:
                if (!paused) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    
                    // Convertir les coordonnées de l'écran en coordonnées du monde
                    Vector2 worldPos = player->getHeadPosition() + 
                                     Vector2(mouseX - screenWidth / 2, mouseY - screenHeight / 2);
                    player->setTarget(worldPos.x, worldPos.y);
                }
                break;
        }
    }
}

void Game::spawnFood() {
    for (auto& food : foods) {
        if (!food.active) {
            Vector2 pos = getRandomMapPosition();
            Food::Type type = static_cast<Food::Type>(foodTypeDist(rng));
            
            // 80% normal, 15% big, 5% golden
            int rand = rng() % 100;
            if (rand < 80) type = Food::NORMAL;
            else if (rand < 95) type = Food::BIG;
            else type = Food::GOLDEN;
            
            food.spawn(pos.x, pos.y, type);
            break;
        }
    }
}

void Game::updateFood() {
    // La nourriture n'a pas besoin de mise à jour particulière
    // Elle est statique jusqu'à être mangée
}

void Game::checkFoodCollisions() {
    for (auto& food : foods) {
        if (food.active && food.isColliding(player->getHeadPosition(), player->radius)) {
            // Le joueur mange la nourriture
            player->grow();
            food.reset();
            
            // Faire apparaître de la nouvelle nourriture
            spawnFood();
        }
    }
}

void Game::checkCollisions() {
    if (!player->alive) return; // Éviter les vérifications répétées
    
    // Collision avec les murs
    if (player->checkWallCollision(screenWidth, screenHeight, mapRadius)) {
        player->alive = false;
        std::cout << "Game Over! Score: " << player->score << std::endl;
        return;
    }
    
    // Collision avec soi-même
    if (player->checkSelfCollision()) {
        player->alive = false;
        std::cout << "Game Over! Score: " << player->score << std::endl;
        return;
    }
    
    // Collision avec la nourriture
    checkFoodCollisions();
}

Vector2 Game::getRandomMapPosition() {
    Vector2 center(screenWidth / 2, screenHeight / 2);
    
    int attempts = 0;
    while (attempts < 50) {
        float angle = angleDist(rng);
        float distance = distanceDist(rng);
        
        Vector2 pos = center + Vector2(
            std::cos(angle) * distance,
            std::sin(angle) * distance
        );
        
        if (isPositionValid(pos, 10.0f)) {
            return pos;
        }
        
        attempts++;
    }
    
    // Si on n'a pas trouvé de position valide, retourner une position aléatoire
    float angle = angleDist(rng);
    float distance = distanceDist(rng);
    return center + Vector2(
        std::cos(angle) * distance,
        std::sin(angle) * distance
    );
}

bool Game::isPositionValid(const Vector2& pos, float radius) {
    // Vérifier que la position est dans la map
    Vector2 center(screenWidth / 2, screenHeight / 2);
    if (pos.distance(center) + radius > mapRadius) {
        return false;
    }
    
    // Vérifier qu'elle n'est pas trop proche du joueur
    if (pos.distance(player->getHeadPosition()) < 50.0f) {
        return false;
    }
    
    // Vérifier qu'elle n'est pas trop proche d'autres nourritures
    for (const auto& food : foods) {
        if (food.active && pos.distance(food.position) < radius + food.radius + 20.0f) {
            return false;
        }
    }
    
    return true;
}

void Game::resetGame() {
    player->reset();
    
    // Réinitialiser toute la nourriture
    for (auto& food : foods) {
        food.reset();
    }
    
    // Faire apparaître de la nouvelle nourriture
    for (int i = 0; i < MAX_FOOD; ++i) {
        spawnFood();
    }
}
