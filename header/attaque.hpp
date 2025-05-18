#ifndef ATTAQUE_HPP
#define ATTAQUE_HPP

#include <string>
#include <vector>

struct Attaque {
    std::string nom;
    std::string type;
    int puissance;
};

// Forward declaration
class Pokemon;

int calculerDegats(const Pokemon& attaquant, const Pokemon& defenseur);
float calculerMultiplicateurType(const std::string& type_attaque, const std::vector<std::string>& types_cible);

#endif
