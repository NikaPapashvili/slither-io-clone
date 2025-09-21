#!/bin/bash

echo "=== Compilation du Slither.io Clone ==="

# Créer le dossier de build
mkdir -p build
cd build

# Configurer avec CMake
echo "Configuration avec CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compiler
echo "Compilation..."
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "✅ Compilation réussie!"
    echo "Lancement du jeu..."
    ./slither
else
    echo "❌ Erreur de compilation!"
    exit 1
fi
