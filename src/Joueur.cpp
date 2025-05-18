#include "Joueur.hpp"
#include <iostream>

Joueur::Joueur(std::string nom)
    : Entraineur(nom), badges(0), victoires(0), defaites(0), phrase("") {}

Joueur::Joueur(std::string nom, int b, int v, int d)
    : Entraineur(nom), badges(b), victoires(v), defaites(d), phrase("") {}

int Joueur::getBadges() const { return badges; }
int Joueur::getVictoires() const { return victoires; }
int Joueur::getDefaites() const { return defaites; }

void Joueur::ajouterVictoire() { victoires++; }
void Joueur::ajouterDefaite() { defaites++; }
void Joueur::ajouterBadge() { badges++; }

void Joueur::afficherStatistiques() const {
    std::cout << "\n📊 Statistiques de " << nom << " :\n";
    std::cout << "   - Badges    : " << badges << "\n";
    std::cout << "   - Victoires : " << victoires << "\n";
    std::cout << "   - Défaites  : " << defaites << "\n";
}

// ✅ méthodes pour phrase
std::string Joueur::getPhrase() const {
    return phrase;
}

void Joueur::setPhrase(const std::string& p) {
    phrase = p;
}

// ✅ méthodes d'accès à l'équipe
std::vector<Pokemon>& Joueur::getEquipe() {
    return equipe;
}

const std::vector<Pokemon>& Joueur::getEquipe() const {
    return equipe;
}
