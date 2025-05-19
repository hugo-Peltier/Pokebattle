#ifndef CHARGEURCSV_HPP
#define CHARGEURCSV_HPP
#include "config.hpp" 
#include "Joueur.hpp"
#include "Leader.hpp"
#include "Maitre.hpp"
#include "Pokemon.hpp"
#include <vector>
#include <string>

std::vector<Pokemon> charger_pokemons(const std::string& chemin);
std::vector<Pokemon> trouver_equipe_pokemon(const std::vector<std::string>& noms);
std::vector<Joueur> charger_joueurs_par_gymnase(const std::string& gymnase);
Leader charger_leader_par_gymnase(const std::string& gymnase);
std::vector<Maitre> charger_maitres();
std::vector<Joueur> charger_tous_les_joueurs();
void sauvegarderJoueur(const Joueur& joueur);

#endif
// @hugop