#include "type_chart.hpp"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <limits>

// Dictionnaires de faiblesses et résistances
static const std::unordered_map<std::string, std::vector<std::string>> FAIBLESSES = {
    {"Feu", {"Eau", "Roche", "Sol"}},
    {"Eau", {"Plante", "Électrik"}},
    {"Plante", {"Feu", "Glace", "Poison", "Vol", "Insecte"}},
    {"Électrik", {"Sol"}},
    {"Glace", {"Feu", "Combat", "Roche", "Acier"}},
    {"Combat", {"Vol", "Psy", "Fée"}},
    {"Poison", {"Sol", "Psy"}},
    {"Sol", {"Eau", "Plante", "Glace"}},
    {"Vol", {"Électrik", "Glace", "Roche"}},
    {"Psy", {"Insecte", "Spectre", "Ténèbres"}},
    {"Insecte", {"Feu", "Vol", "Roche"}},
    {"Roche", {"Eau", "Plante", "Combat", "Sol", "Acier"}},
    {"Spectre", {"Spectre", "Ténèbres"}},
    {"Dragon", {"Glace", "Dragon", "Fée"}},
    {"Ténèbres", {"Combat", "Insecte", "Fée"}},
    {"Acier", {"Feu", "Combat", "Sol"}},
    {"Fée", {"Poison", "Acier"}}
};

static const std::unordered_map<std::string, std::vector<std::string>> RESISTANCES = {
    {"Feu", {"Plante", "Glace", "Insecte", "Acier", "Fée"}},
    {"Eau", {"Feu", "Eau", "Glace", "Acier"}},
    {"Plante", {"Eau", "Sol", "Roche"}},
    {"Électrik", {"Vol", "Acier", "Électrik"}},
    {"Glace", {"Glace"}},
    {"Combat", {"Roche", "Insecte", "Ténèbres"}},
    {"Poison", {"Plante", "Fée", "Combat", "Poison", "Insecte"}},
    {"Sol", {"Poison", "Roche"}},
    {"Vol", {"Plante", "Combat", "Insecte"}},
    {"Psy", {"Combat", "Psy"}},
    {"Insecte", {"Plante", "Combat", "Sol"}},
    {"Roche", {"Feu", "Vol", "Poison", "Normal"}},
    {"Spectre", {"Poison", "Insecte"}},
    {"Dragon", {"Feu", "Eau", "Électrik", "Plante"}},
    {"Ténèbres", {"Spectre", "Psy", "Ténèbres"}},
    {"Acier", {"Normal", "Plante", "Glace", "Vol", "Psy", "Insecte", "Roche", "Dragon", "Acier", "Fée"}},
    {"Fée", {"Combat", "Insecte", "Ténèbres", "Dragon"}}
};

// Fonction appelée en combat
float calculerMultiplicateurType(const std::string& type_attaque, const std::vector<std::string>& types_defenseur) {
    float total = 1.0f;
    for (const auto& type_def : types_defenseur) {
        if (FAIBLESSES.count(type_def)) {
            const auto& faibles = FAIBLESSES.at(type_def);
            if (std::find(faibles.begin(), faibles.end(), type_attaque) != faibles.end())
                total *= 2.0f;
        }
        if (RESISTANCES.count(type_def)) {
            const auto& resists = RESISTANCES.at(type_def);
            if (std::find(resists.begin(), resists.end(), type_attaque) != resists.end())
                total *= 0.5f;
        }
    }
    return total;
}

// Affichage simple du tableau
void afficher_tableau_types() {
    std::vector<std::string> types = {
        "Feu", "Eau", "Plante", "Électrik", "Glace", "Combat", "Poison",
        "Sol", "Vol", "Psy", "Insecte", "Roche", "Spectre",
        "Dragon", "Ténèbres", "Acier", "Fée"
    };

    std::cout << std::left
              << std::setw(12) << "TYPE"
              << std::setw(45) << "FAIBLESSES (x2)"
              << std::setw(45) << "RESISTANCES (x0.5)" << "\n";

    std::cout << std::string(102, '-') << "\n";

    for (const auto& type : types) {
        std::string faibles = "—";
        std::string resist = "—";

        if (FAIBLESSES.count(type)) {
            faibles = "";
            for (const auto& t : FAIBLESSES.at(type))
                faibles += t + ", ";
            if (!faibles.empty()) faibles.pop_back(), faibles.pop_back();
        }

        if (RESISTANCES.count(type)) {
            resist = "";
            for (const auto& t : RESISTANCES.at(type))
                resist += t + ", ";
            if (!resist.empty()) resist.pop_back(), resist.pop_back();
        }

        std::cout << std::setw(12) << type
                  << std::setw(45) << faibles
                  << std::setw(45) << resist << "\n";
    }
    std::cout << "\nAppuyez sur Entrée pour revenir au menu...";
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // vide le buffer
std::cin.get();  // attend Entrée

}
