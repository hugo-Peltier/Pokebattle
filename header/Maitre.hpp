#ifndef MAITRE_HPP
#define MAITRE_HPP

#include "Entraineur.hpp"

class Maitre : public Entraineur {
private:
    std::string capacite_speciale;

public:
    Maitre(std::string nom, std::string capacite_speciale, const std::vector<std::string>& equipe);
    std::string getCapacite() const;
    void boosterEquipe();  
};

#endif
// @hugop