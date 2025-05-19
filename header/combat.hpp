#pragma once

#include "Joueur.hpp"
#include "Leader.hpp"
#include "Maitre.hpp"
#include <string>
#include <vector>

namespace ftxui {
    class ScreenInteractive;
}
bool lancer_combat_contre(Joueur& joueur, const std::string& nom_adv, const std::vector<std::string>& equipe_adv_noms);
void lancer_combat_gymnase(Joueur& joueur, const std::string& gymnase);
void lancer_combat_gymnase(Joueur& joueur, const Leader& leader, const std::vector<Joueur>& adversaires);
void lancer_combat_maitre(Joueur& joueur, const Maitre& maitre);
bool demander_confirmation(const std::string& message);
// @hugop