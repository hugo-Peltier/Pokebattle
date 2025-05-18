#include "Entraineur.hpp"
#include <iostream>

Entraineur::Entraineur(std::string nom) : nom(nom) {}

std::string Entraineur::getNom() const {
    return nom;
}

const std::vector<Pokemon>& Entraineur::getEquipe() const {
    return equipe;
}

void Entraineur::ajouterPokemon(const Pokemon& p) {
    if (equipe.size() < 6) {
        equipe.push_back(p);
    }
}

void Entraineur::soignerEquipe() {
    for (auto& p : equipe) {
        p.soigner();
    }
}

bool Entraineur::equipeVide() const {
    return equipe.empty();
}

void Entraineur::interagir() const {
    std::cout << "👋 " << nom << " vous salue." << std::endl;
}
