#ifndef TYPE_CHART_HPP
#define TYPE_CHART_HPP

#include <string>
#include <vector>

// Calcule le multiplicateur de dégâts basé sur le type de l'attaque
// et la liste des types du Pokémon défenseur (x2, x0.5, etc.)
float calculerMultiplicateurType(const std::string& type_attaque, const std::vector<std::string>& types_defenseur);
void afficher_tableau_types();


#endif
