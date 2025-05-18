#pragma once

#include "Joueur.hpp"
#include "Leader.hpp"
#include "Maitre.hpp"
#include <string>
#include <vector>

namespace ftxui {
    class ScreenInteractive;
}

// Combat tour par tour contre un adversaire (équipe complète)
bool lancer_combat_contre(Joueur& joueur, const std::string& nom_adv, const std::vector<std::string>& equipe_adv_noms);

// Combat contre un gymnase (en spécifiant son nom)
void lancer_combat_gymnase(Joueur& joueur, const std::string& gymnase);

// Variante : gymnase avec objets Leader + liste de joueurs adversaires
void lancer_combat_gymnase(Joueur& joueur, const Leader& leader, const std::vector<Joueur>& adversaires);

// Combat contre un Maître Pokémon
void lancer_combat_maitre(Joueur& joueur, const Maitre& maitre);

// Affichage de confirmation (retourne vrai si "Oui" est sélectionné)
bool demander_confirmation(const std::string& message);
