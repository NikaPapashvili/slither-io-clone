#include "Food.h"
#include <cmath>
#include <algorithm>

Food::Food() 
    : position(0, 0), radius(5.0f), active(false), value(10), type(NORMAL),
      rng(std::random_device{}()), colorDist(0.0f, 1.0f), positionDist(-1.0f, 1.0f) {
    
    color = {255, 100, 100, 255}; // Rouge par défaut
}

void Food::spawn(float x, float y, Type foodType) {
    position = Vector2(x, y);
    type = foodType;
    active = true;
    
    // Configurer selon le type
    switch (type) {
        case NORMAL:
            radius = 5.0f;
            value = 10;
            setRandomColor();
            break;
        case BIG:
            radius = 8.0f;
            value = 25;
            color = {255, 150, 50, 255}; // Orange
            break;
        case GOLDEN:
            radius = 6.0f;
            value = 50;
            color = {255, 215, 0, 255}; // Or
            break;
    }
}

void Food::reset() {
    active = false;
    position = Vector2(0, 0);
}

void Food::render(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Vector2& playerPos) {
    if (!active) return;
    
    // Calculer la position relative à la caméra
    Vector2 screenPos = position - playerPos + Vector2(screenWidth / 2, screenHeight / 2);
    
    // Vérifier si la nourriture est visible à l'écran
    if (screenPos.x < -radius || screenPos.x > screenWidth + radius ||
        screenPos.y < -radius || screenPos.y > screenHeight + radius) {
        return;
    }
    
    // Dessiner la nourriture
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    int centerX = (int)screenPos.x;
    int centerY = (int)screenPos.y;
    int radiusInt = (int)radius;
    
    // Dessiner un cercle
    for (int x = -radiusInt; x <= radiusInt; ++x) {
        for (int y = -radiusInt; y <= radiusInt; ++y) {
            if (x * x + y * y <= radiusInt * radiusInt) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
    
    // Ajouter un effet de brillance pour la nourriture dorée
    if (type == GOLDEN) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 200, 150);
        int innerRadius = radiusInt / 2;
        for (int x = -innerRadius; x <= innerRadius; ++x) {
            for (int y = -innerRadius; y <= innerRadius; ++y) {
                if (x * x + y * y <= innerRadius * innerRadius) {
                    SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
                }
            }
        }
    }
}

bool Food::isColliding(const Vector2& playerPos, float playerRadius) const {
    if (!active) return false;
    return position.distance(playerPos) < radius + playerRadius;
}

void Food::setRandomColor() {
    // Couleurs vives et variées
    int colorIndex = rng() % 6;
    switch (colorIndex) {
        case 0: color = {255, 100, 100, 255}; break; // Rouge
        case 1: color = {100, 255, 100, 255}; break; // Vert
        case 2: color = {100, 100, 255, 255}; break; // Bleu
        case 3: color = {255, 255, 100, 255}; break; // Jaune
        case 4: color = {255, 100, 255, 255}; break; // Magenta
        case 5: color = {100, 255, 255, 255}; break; // Cyan
    }
}

void Food::setRandomPosition(int screenWidth, int screenHeight, float mapRadius) {
    Vector2 center(screenWidth / 2, screenHeight / 2);
    
    // Générer une position aléatoire dans le cercle de la map
    float angle = colorDist(rng) * 2.0f * M_PI;
    float distance = colorDist(rng) * (mapRadius - radius - 50); // Laisser de la marge
    
    position = center + Vector2(
        std::cos(angle) * distance,
        std::sin(angle) * distance
    );
}
