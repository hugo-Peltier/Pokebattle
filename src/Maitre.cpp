#include "Maitre.hpp"
#include "ChargeurCSV.hpp"
#include <iostream>

Maitre::Maitre(std::string nom, std::string capacite, const std::vector<std::string>& noms_pokemons)
    : Entraineur(nom), capacite_speciale(capacite) {
    auto equipe_chargee = trouver_equipe_pokemon(noms_pokemons);
    for (auto& p : equipe_chargee) {
        ajouterPokemon(p);
    }
}

std::string Maitre::getCapacite() const {
    return capacite_speciale;
}

void Maitre::boosterEquipe() {
    for (auto& p : equipe) {
        if (!p.getAttaques().empty()) {
            // @hugop
            Attaque& atk = p.getAttaques()[0];
            int ancienne_puissance = atk.puissance;
            atk.puissance = static_cast<int>(atk.puissance * 1.25f);
            std::cout << "⚡ " << p.getNom() << " voit son attaque " << atk.nom
                      << " passer de " << ancienne_puissance << " à " << atk.puissance << " dégâts.\n";
        }
    }
}
// @hugop