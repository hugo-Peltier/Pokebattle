#ifndef JOUEUR_HPP
#define JOUEUR_HPP

#include "Entraineur.hpp"

class Joueur : public Entraineur {
private:
    int badges;
    int victoires;
    int defaites;
    std::string phrase; 

public:
    Joueur(std::string nom);
    Joueur(std::string nom, int badges, int victoires, int defaites);

    int getBadges() const;
    int getVictoires() const;
    int getDefaites() const;

    void ajouterVictoire();
    void ajouterDefaite();
    void ajouterBadge();
    void afficherStatistiques() const;

    std::string getPhrase() const;          
    void setPhrase(const std::string& p);    
    std::vector<Pokemon>& getEquipe();                  
    const std::vector<Pokemon>& getEquipe() const;       
};

#endif
// @hugop