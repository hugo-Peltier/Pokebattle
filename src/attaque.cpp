#include "attaque.hpp"
#include "Pokemon.hpp"
#include "type_chart.hpp"

int calculerDegats(const Pokemon& attaquant, const Pokemon& defenseur) {
    if (attaquant.getAttaques().empty()) return 0;
    const Attaque& attaque = attaquant.getAttaques()[0];
    float multiplicateur = calculerMultiplicateurType(attaque.type, defenseur.getTypes());
    return attaque.puissance * multiplicateur;
}
// @hugop
