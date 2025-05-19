#include "Leader.hpp"
#include "ChargeurCSV.hpp"

Leader::Leader(std::string nom, std::string gymnase, std::string badge,
               const std::vector<std::string>& noms_pokemons)
    : Entraineur(nom), gymnase(gymnase), badge(badge) {
    auto equipe_chargee = trouver_equipe_pokemon(noms_pokemons);
    for (const auto& p : equipe_chargee) {
        ajouterPokemon(p);
    }
}

std::string Leader::getGymnase() const { return gymnase; }
std::string Leader::getBadge() const { return badge; }
// @hugop