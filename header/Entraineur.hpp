#ifndef ENTRAINEUR_HPP
#define ENTRAINEUR_HPP

#include <string>
#include <vector>
#include <memory>
#include "Pokemon.hpp"
#include "Interagir.hpp"

class Entraineur : public Interagir {
protected:
    std::string nom;
    std::vector<Pokemon> equipe;

public:
    Entraineur(std::string nom);
    virtual ~Entraineur() = default;

    std::string getNom() const;
    const std::vector<Pokemon>& getEquipe() const;
    void ajouterPokemon(const Pokemon& p);
    void soignerEquipe();
    bool equipeVide() const;

    void interagir() const override;
};

#endif
