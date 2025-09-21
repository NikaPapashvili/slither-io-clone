# Slither.io Clone

Un clone de Slither.io développé en C++ avec SDL2.

## Fonctionnalités

- 🐍 Serpent qui grandit en mangeant de la nourriture
- 🎯 Contrôles fluides à la souris
- 🍎 Différents types de nourriture (normale, grosse, dorée)
- 🗺️ Carte ronde avec limites
- 💥 Détection de collision (murs et soi-même)
- 🎨 Design moderne et coloré

## Prérequis

- CMake 3.16+
- Compilateur C++17 (GCC, Clang, MSVC)
- SDL2
- SDL2_image
- SDL2_ttf

### Installation des dépendances (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install cmake build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

### Installation des dépendances (Arch Linux)

```bash
sudo pacman -S cmake gcc sdl2 sdl2_image sdl2_ttf
```

## Compilation et exécution

```bash
# Méthode simple
./build.sh

# Ou manuellement
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
./slither
```

## Contrôles

- **Souris** : Diriger le serpent
- **R** : Recommencer le jeu
- **P** : Pause/Reprendre
- **Échap** : Quitter

## Architecture

- `src/Vector2.h/cpp` : Classe de vecteur 2D
- `src/Player.h/cpp` : Logique du joueur/serpent
- `src/Food.h/cpp` : Système de nourriture
- `src/Game.h/cpp` : Moteur de jeu principal
- `src/GameRenderer.h/cpp` : Rendu graphique
- `src/main.cpp` : Point d'entrée

## Performance

Le jeu est optimisé pour gérer des centaines d'objets simultanément et peut facilement être étendu pour supporter du multijoueur.
