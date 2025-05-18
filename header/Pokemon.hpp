#ifndef POKEMON_HPP
#define POKEMON_HPP

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "attaque.hpp"

class Pokemon {
private:
    std::string nom;
    std::vector<std::string> types;
    int hp;
    int hp_initial;
    std::vector<Attaque> attaques;

public:
    // Constructeurs
    Pokemon() = default;

    Pokemon(const std::string& nom, const std::vector<std::string>& types, int hp, const std::vector<Attaque>& attaques)
        : nom(nom), types(types), hp(hp), hp_initial(hp), attaques(attaques) {}

    // Accesseurs
    std::string getNom() const { return nom; }
    std::vector<std::string> getTypes() const { return types; }
    int getHP() const { return hp; }
    int getHPInitial() const { return hp_initial; }
    std::vector<Attaque> getAttaques() const { return attaques; }

    // Méthodes de gestion du combat
    void recevoirDegats(int degats) { hp = std::max(0, hp - degats); }
    bool estKo() const { return hp <= 0; }
    void soigner() { hp = hp_initial; }

    // Méthode d’interaction
    void interagir() const {
        std::cout << nom << " vous regarde avec curiosité.\n";
    }
};

#endif
