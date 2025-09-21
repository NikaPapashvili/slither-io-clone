#include "Player.h"
#include <algorithm>
#include <cmath>

Player::Player(float x, float y) 
    : position(x, y), velocity(0, 0), target(x, y), 
      speed(150.0f), radius(8.0f), score(0), alive(true) {
    
    // Couleur du joueur (gradient vert-bleu)
    color = {50, 255, 100, 255};
    
    // Initialiser le corps avec quelques segments
    body.push_back(position);
    body.push_back(position - Vector2(20, 0));
    body.push_back(position - Vector2(40, 0));
}

void Player::update(float deltaTime) {
    if (!alive) return;

    // Calculer la direction vers la cible
    Vector2 direction = target - position;
    if (direction.length() > 5.0f) {
        direction.normalize();
        velocity = direction * speed;
    } else {
        velocity = Vector2(0, 0);
    }

    // Mettre à jour la position
    position += velocity * deltaTime;
    
    // Mettre à jour le corps
    updateBody();
}

void Player::setTarget(float x, float y) {
    target = Vector2(x, y);
}

void Player::grow() {
    score += 10;
    
    // Ajouter un nouveau segment à la fin du corps
    if (body.size() > 1) {
        Vector2 lastSegment = body.back();
        Vector2 secondLastSegment = body[body.size() - 2];
        Vector2 direction = (lastSegment - secondLastSegment).normalized();
        
        // Ajouter le nouveau segment dans la direction opposée
        Vector2 newSegment = lastSegment + direction * 15.0f;
        body.push_back(newSegment);
    } else {
        // Si c'est le premier segment, l'ajouter derrière la tête
        Vector2 direction = (body[0] - target).normalized();
        Vector2 newSegment = body[0] + direction * 15.0f;
        body.push_back(newSegment);
    }
    
    // Augmenter légèrement la vitesse
    speed = std::min(speed + 2.0f, 200.0f);
}

void Player::reset() {
    position = Vector2(400, 300);
    target = position;
    velocity = Vector2(0, 0);
    speed = 150.0f;
    score = 0;
    alive = true;
    
    // Réinitialiser le corps
    body.clear();
    body.push_back(position);
    body.push_back(position - Vector2(20, 0));
    body.push_back(position - Vector2(40, 0));
}

void Player::render(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
    if (!alive) return;

    // Rendre le corps du serpent
    for (size_t i = 0; i < body.size(); ++i) {
        Vector2 segmentPos = body[i];
        
        // Calculer la position relative à la caméra (le joueur est au centre)
        Vector2 screenPos = segmentPos - position + Vector2(screenWidth / 2, screenHeight / 2);
        
        // Calculer la taille du segment (plus petit vers la queue)
        float segmentRadius = radius * (1.0f - (float)i / (body.size() * 2.0f));
        segmentRadius = std::max(segmentRadius, 3.0f);
        
        // Couleur dégradée
        SDL_Color segmentColor = color;
        segmentColor.a = 255 - (i * 50);
        if (segmentColor.a < 100) segmentColor.a = 100;
        
        // Dessiner le segment
        SDL_SetRenderDrawColor(renderer, segmentColor.r, segmentColor.g, segmentColor.b, segmentColor.a);
        
        // Dessiner un cercle approximatif avec des points
        int centerX = (int)screenPos.x;
        int centerY = (int)screenPos.y;
        int radiusInt = (int)segmentRadius;
        
        for (int x = -radiusInt; x <= radiusInt; ++x) {
            for (int y = -radiusInt; y <= radiusInt; ++y) {
                if (x * x + y * y <= radiusInt * radiusInt) {
                    SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
                }
            }
        }
    }
}

bool Player::checkSelfCollision() const {
    if (body.size() < 4) return false;
    
    Vector2 head = body[0];
    float headRadius = radius;
    
    // Vérifier la collision avec les segments du corps (en évitant les premiers)
    for (size_t i = 4; i < body.size(); ++i) {
        float distance = head.distance(body[i]);
        float segmentRadius = radius * (1.0f - (float)i / (body.size() * 2.0f));
        segmentRadius = std::max(segmentRadius, 3.0f);
        
        if (distance < headRadius + segmentRadius) {
            return true;
        }
    }
    
    return false;
}

bool Player::checkWallCollision(int screenWidth, int screenHeight, float mapRadius) const {
    Vector2 center(screenWidth / 2, screenHeight / 2);
    float distanceFromCenter = position.distance(center);
    
    return distanceFromCenter + radius > mapRadius;
}

bool Player::checkFoodCollision(const Vector2& foodPos, float foodRadius) const {
    return position.distance(foodPos) < radius + foodRadius;
}

float Player::getBodyRadius() const {
    return radius;
}

Vector2 Player::getHeadPosition() const {
    return body.empty() ? position : body[0];
}

void Player::updateBody() {
    if (body.empty()) return;
    
    // Mettre à jour la position de la tête
    body[0] = position;
    
    // Faire suivre les segments
    for (size_t i = 1; i < body.size(); ++i) {
        Vector2 direction = body[i - 1] - body[i];
        if (direction.length() > 15.0f) {
            direction.normalize();
            body[i] = body[i - 1] - direction * 15.0f;
        }
    }
}

void Player::limitToMap(int screenWidth, int screenHeight, float mapRadius) {
    Vector2 center(screenWidth / 2, screenHeight / 2);
    Vector2 direction = position - center;
    
    if (direction.length() + radius > mapRadius) {
        direction.normalize();
        position = center + direction * (mapRadius - radius);
    }
}
