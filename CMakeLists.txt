cmake_minimum_required(VERSION 3.16)
project(SlitherClone)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver pkg-config
find_package(PkgConfig REQUIRED)

# Trouver SDL2 avec pkg-config
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

# Inclure les répertoires
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})

# Créer l'exécutable
add_executable(slither
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Food.cpp
    src/Vector2.cpp
    src/GameRenderer.cpp
)

# Lier les bibliothèques
target_link_libraries(slither ${SDL2_LIBRARIES})
target_link_libraries(slither ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(slither ${SDL2_TTF_LIBRARIES})

# Compiler avec optimisations en mode Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_BUILD_TYPE Release)
