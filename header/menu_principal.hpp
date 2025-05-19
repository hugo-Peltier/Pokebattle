#ifndef MENU_PRINCIPAL_HPP
#define MENU_PRINCIPAL_HPP

#include "Joueur.hpp"

namespace ui {
    void menu_principal(Joueur& joueur);
    void sauvegarderJoueurTemporaire(const Joueur& joueur);
    void supprimerJoueurTemporaire(const Joueur& joueur);
}

#endif
// @hugop