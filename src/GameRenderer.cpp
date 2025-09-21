#include "GameRenderer.h"
#include <iostream>
#include <cmath>

GameRenderer& GameRenderer::getInstance() {
    static GameRenderer instance;
    return instance;
}

bool GameRenderer::initialize(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    
    // Créer la fenêtre
    window = SDL_CreateWindow(
        "Slither.io Clone",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        std::cerr << "Erreur création fenêtre: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Créer le renderer
    renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (!renderer) {
        std::cerr << "Erreur création renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialiser TTF
    if (TTF_Init() == -1) {
        std::cerr << "Erreur TTF: " << TTF_GetError() << std::endl;
        return false;
    }
    
    // Charger la police
    font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 24);
    if (!font) {
        // Essayer une police alternative
        font = TTF_OpenFont("/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf", 24);
        if (!font) {
            std::cerr << "Erreur chargement police: " << TTF_GetError() << std::endl;
            // Continuer sans police personnalisée
        }
    }
    
    // Définir les couleurs
    backgroundColor = {26, 26, 46, 255};      // Bleu foncé
    mapColor = {15, 52, 96, 255};            // Bleu moyen
    textColor = {255, 255, 255, 255};        // Blanc
    gameOverColor = {255, 100, 100, 255};    // Rouge
    
    return true;
}

void GameRenderer::cleanup() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    TTF_Quit();
}

void GameRenderer::beginFrame() {
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);
}

void GameRenderer::endFrame() {
    SDL_RenderPresent(renderer);
}

void GameRenderer::renderMap(int screenWidth, int screenHeight, float mapRadius) {
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    int radius = (int)mapRadius;
    
    // Dessiner le cercle de la map avec un effet de gradient
    for (int r = radius; r >= radius - 10; r -= 2) {
        SDL_Color color = mapColor;
        color.a = 255 - (radius - r) * 20;
        drawCircle(centerX, centerY, r, color, false);
    }
    
    // Dessiner des lignes de grille pour l'effet
    SDL_SetRenderDrawColor(renderer, mapColor.r, mapColor.g, mapColor.b, 30);
    for (int i = 0; i < 8; ++i) {
        float angle = i * M_PI / 4.0f;
        int x1 = centerX + (int)(std::cos(angle) * (radius - 50));
        int y1 = centerY + (int)(std::sin(angle) * (radius - 50));
        int x2 = centerX + (int)(std::cos(angle) * radius);
        int y2 = centerY + (int)(std::sin(angle) * radius);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void GameRenderer::renderUI(int score, bool alive) {
    // Afficher le score
    std::string scoreText = "Score: " + std::to_string(score);
    drawText(scoreText, 20, 20, textColor, 24);
    
    // Afficher Game Over si le joueur est mort
    if (!alive) {
        drawText("GAME OVER", screenWidth / 2 - 100, screenHeight / 2 - 50, gameOverColor, 48);
        drawText("Appuyez sur R pour recommencer", screenWidth / 2 - 150, screenHeight / 2 + 20, textColor, 18);
    }
    
    // Instructions
    drawText("Utilisez la souris pour diriger", 20, screenHeight - 60, {textColor.r, textColor.g, textColor.b, 150}, 16);
    drawText("Mangez les points pour grandir!", 20, screenHeight - 40, {textColor.r, textColor.g, textColor.b, 150}, 16);
}

void GameRenderer::drawCircle(int centerX, int centerY, int radius, SDL_Color color, bool filled) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    if (filled) {
        // Cercle rempli
        for (int x = -radius; x <= radius; ++x) {
            for (int y = -radius; y <= radius; ++y) {
                if (x * x + y * y <= radius * radius) {
                    SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
                }
            }
        }
    } else {
        // Cercle vide (approximation avec des points)
        for (int angle = 0; angle < 360; angle += 2) {
            int x = centerX + (int)(radius * std::cos(angle * M_PI / 180.0));
            int y = centerY + (int)(radius * std::sin(angle * M_PI / 180.0));
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void GameRenderer::drawText(const std::string& text, int x, int y, SDL_Color color, int fontSize) {
    if (!font) {
        // Si pas de police, dessiner des rectangles pour simuler du texte
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect rect = {x, y, (int)text.length() * 8, 20};
        SDL_RenderFillRect(renderer, &rect);
        return;
    }
    
    // Créer la surface de texte
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        return;
    }
    
    // Créer la texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_FreeSurface(textSurface);
        return;
    }
    
    // Rendre la texture
    SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);
    
    // Nettoyer
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}
