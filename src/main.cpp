#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "=== Slither.io Clone ===" << std::endl;
    std::cout << "Démarrage du jeu..." << std::endl;
    
    // Créer le jeu
    Game game(1200, 800);
    
    // Initialiser
    if (!game.initialize()) {
        std::cerr << "Erreur d'initialisation du jeu!" << std::endl;
        return -1;
    }
    
    std::cout << "Jeu initialisé avec succès!" << std::endl;
    std::cout << "Contrôles:" << std::endl;
    std::cout << "  - Souris: Diriger le serpent" << std::endl;
    std::cout << "  - R: Recommencer" << std::endl;
    std::cout << "  - P: Pause" << std::endl;
    std::cout << "  - Échap: Quitter" << std::endl;
    
    // Lancer la boucle principale
    game.run();
    
    // Nettoyer
    game.cleanup();
    
    std::cout << "Jeu terminé. Merci d'avoir joué!" << std::endl;
    return 0;
}
