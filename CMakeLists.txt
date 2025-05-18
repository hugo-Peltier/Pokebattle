cmake_minimum_required(VERSION 3.14)
project(Pokebattle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dossiers d'includes
include_directories(${CMAKE_SOURCE_DIR}/header)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/ui)

# 🔤 Forcer l'encodage UTF-8 à la compilation
add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)

# Fichiers sources
add_executable(Pokebattle
    src/main.cpp
    src/Entraineur.cpp
    src/Joueur.cpp
    src/Leader.cpp
    src/Maitre.cpp
    src/ChargeurCSV.cpp
    src/tutoriel.cpp
    src/combat.cpp
    src/type_chart.cpp      
    src/attaque.cpp
    src/ui/intro.cpp
    src/ui/menu_principal.cpp
    src/ui/menu_combat.cpp
    src/ui/credits.cpp

)

# FTXUI requis
find_package(ftxui REQUIRED)
target_link_libraries(Pokebattle PRIVATE ftxui::screen ftxui::dom ftxui::component)

# SFML pour le son
find_package(SFML 2.5 COMPONENTS audio REQUIRED)
target_link_libraries(Pokebattle PRIVATE sfml-audio)

# 📁 Audio (juste pour debug)
file(GLOB AUDIO_FILES ${CMAKE_SOURCE_DIR}/audio/*.wav)
message(STATUS "📁 Dossier audio utilisé : ${CMAKE_SOURCE_DIR}/audio/")

# ✅ Copier automatiquement le dossier `data/` vers `build/data/`
#file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
