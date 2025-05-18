#include "ChargeurCSV.hpp"
#include "attaque.hpp"
#include "config.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

// Fonction utilitaire de normalisation des noms
std::string normaliser_nom(const std::string& s) {
    std::string resultat = s;
    std::vector<std::pair<std::string, std::string>> remplacements = {
        {"é", "e"}, {"è", "e"}, {"ê", "e"}, {"ë", "e"},
        {"à", "a"}, {"â", "a"}, {"ä", "a"},
        {"î", "i"}, {"ï", "i"},
        {"ô", "o"}, {"ö", "o"},
        {"û", "u"}, {"ü", "u"},
        {"ç", "c"},
        {"’", ""}, {"'", ""}, {" ", ""}
    };
    for (const auto& [old, rep] : remplacements) {
        size_t pos = 0;
        while ((pos = resultat.find(old, pos)) != std::string::npos) {
            resultat.replace(pos, old.length(), rep);
        }
    }
    std::transform(resultat.begin(), resultat.end(), resultat.begin(), ::tolower);
    return resultat;
}

// Chargement des Pokémon depuis le CSV
std::vector<Pokemon> charger_pokemons(const std::string& chemin) {
    std::vector<Pokemon> pokemons;
    std::ifstream fichier(chemin);
    std::string ligne;

    if (!fichier.is_open()) {
        std::cerr << "[ERREUR] Impossible d'ouvrir le fichier : " << chemin << "\n";
        return pokemons;
    }

    std::getline(fichier, ligne); // ignorer l'en-tête

    int ligne_num = 1;
    while (std::getline(fichier, ligne)) {
        ligne_num++;
        std::stringstream ss(ligne);
        std::string nom, type1, type2, hp_str;
        std::string atk1, pow1_str, atk2, pow2_str;

        std::getline(ss, nom, ';');
        std::getline(ss, type1, ';');
        std::getline(ss, type2, ';');
        std::getline(ss, hp_str, ';');
        std::getline(ss, atk1, ';');
        std::getline(ss, pow1_str, ';');
        std::getline(ss, atk2, ';');
        std::getline(ss, pow2_str, ';');

        if (nom.empty() || atk1.empty() || hp_str.empty() || pow1_str.empty()) {
            std::cerr << "[AVERTISSEMENT] Ligne ignorée (champ(s) manquant(s)) ligne " << ligne_num << " : " << ligne << "\n";
            continue;
        }

        int hp = 0, pow1 = 0, pow2 = 0;

        try {
            hp = std::stoi(hp_str);
        } catch (...) {
            std::cerr << "[ERREUR CSV] HP invalide pour " << nom << " ligne " << ligne_num << " : '" << hp_str << "'\n";
            continue;
        }

        try {
            pow1 = std::stoi(pow1_str);
        } catch (...) {
            std::cerr << "[ERREUR CSV] Puissance 1 invalide pour " << nom << " ligne " << ligne_num << " : '" << pow1_str << "'\n";
            continue;
        }

        try {
            pow2 = std::stoi(pow2_str);
        } catch (...) {
            std::cerr << "[AVERTISSEMENT] Puissance 2 invalide pour " << nom << " ligne " << ligne_num << " : '" << pow2_str << "' (valeur 0 utilisée)\n";
            pow2 = 0; // on accepte puissance 2 vide
        }

        std::vector<std::string> types = { type1 };
        if (!type2.empty()) types.push_back(type2);

        std::vector<Attaque> attaques = {
            { atk1, type1, pow1 },
            { atk2, type1, pow2 }
        };

        pokemons.emplace_back(nom, types, hp, attaques);
    }

    return pokemons;
}




// Recherche des Pokémon à partir de noms
std::vector<Pokemon> trouver_equipe_pokemon(const std::vector<std::string>& noms) {
    std::vector<Pokemon> equipe;
    auto tous = charger_pokemons(PATH_POKEMON);
    for (const auto& nom : noms) {
        auto it = std::find_if(tous.begin(), tous.end(), [&](const Pokemon& p) {
            return normaliser_nom(p.getNom()) == normaliser_nom(nom);
        });
        if (it != tous.end()) equipe.push_back(*it);
    }
    return equipe;
}

// Chargement des joueurs
std::vector<Joueur> charger_joueurs_par_gymnase(const std::string& gymnase) {
    std::vector<Joueur> joueurs;
    std::ifstream fichier(PATH_JOUEURS);
    std::string ligne;

    std::getline(fichier, ligne); // Ignorer l'en-tête

    while (std::getline(fichier, ligne)) {
        std::stringstream ss(ligne);
        std::string nom, contree, badges_str, victoires_str, defaites_str;

        std::getline(ss, nom, ',');
        std::getline(ss, contree, ',');
        std::getline(ss, badges_str, ',');
        std::getline(ss, victoires_str, ',');
        std::getline(ss, defaites_str, ',');

        int b = badges_str.empty() ? 0 : std::stoi(badges_str);
        int v = victoires_str.empty() ? 0 : std::stoi(victoires_str);
        int d = defaites_str.empty() ? 0 : std::stoi(defaites_str);

        std::vector<std::string> noms_gymnases = {
            "Carmin sur Mer", "Celadopole", "Safrania", "CramoisIle"
        };

        if (gymnase == "ALL") {
            if (std::find(noms_gymnases.begin(), noms_gymnases.end(), contree) != noms_gymnases.end())
                continue;
        } else if (contree != gymnase) {
            continue;
        }

        Joueur joueur(nom, b, v, d);
        std::string nom_poke;
        while (std::getline(ss, nom_poke, ',')) {
            auto equipe = trouver_equipe_pokemon({nom_poke});
            if (!equipe.empty()) joueur.ajouterPokemon(equipe[0]);
        }

        joueurs.push_back(joueur);
    }

    return joueurs;
}

std::vector<Joueur> charger_tous_les_joueurs() {
    std::vector<Joueur> joueurs;
    std::ifstream fichier(PATH_JOUEURS);
    std::string ligne;

    if (!fichier.is_open()) return joueurs;

    std::getline(fichier, ligne); // Ignorer l'en-tête

    while (std::getline(fichier, ligne)) {
        std::stringstream ss(ligne);
        std::string nom, badges_str, victoires_str, defaites_str, champ;

        std::getline(ss, nom, ';');
        std::getline(ss, badges_str, ';');
        std::getline(ss, victoires_str, ';');
        std::getline(ss, defaites_str, ';');

        if (nom.empty()) continue;

        int b = badges_str.empty() ? 0 : std::stoi(badges_str);
        int v = victoires_str.empty() ? 0 : std::stoi(victoires_str);
        int d = defaites_str.empty() ? 0 : std::stoi(defaites_str);

        Joueur joueur(nom, b, v, d);

        std::vector<std::string> noms_pokemons;
        for (int i = 0; i < 6; ++i) {
            if (!std::getline(ss, champ, ';')) break;
            if (!champ.empty()) noms_pokemons.push_back(champ);
        }

        if (std::getline(ss, champ)) {
            joueur.setPhrase(champ);
        } else {
            joueur.setPhrase("");
        }

        auto equipe = trouver_equipe_pokemon(noms_pokemons);
        for (const auto& p : equipe) joueur.ajouterPokemon(p);

        joueurs.push_back(joueur);
    }

    return joueurs;
}

// Chargement du Leader
Leader charger_leader_par_gymnase(const std::string& gymnase) {
    std::ifstream fichier(PATH_LEADERS);
    std::string ligne;
    if (!fichier.is_open()) return Leader("Inconnu", gymnase, "", {});

    std::getline(fichier, ligne); // Ignorer l'en-tête

    auto normaliser = [](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        std::vector<std::pair<std::string, std::string>> remplacements = {
            {"é", "e"}, {"è", "e"}, {"ê", "e"}, {"ë", "e"},
            {"à", "a"}, {"â", "a"}, {"ä", "a"},
            {"î", "i"}, {"ï", "i"},
            {"ô", "o"}, {"ö", "o"},
            {"û", "u"}, {"ü", "u"},
            {"ç", "c"},
            {"’", ""}, {"'", ""}, {" ", ""}
        };
        for (const auto& [accentue, normal] : remplacements) {
            size_t pos = 0;
            while ((pos = s.find(accentue, pos)) != std::string::npos) {
                s.replace(pos, accentue.length(), normal);
            }
        }
        return s;
    };

    const std::string gymnase_net = normaliser(gymnase);

    while (std::getline(fichier, ligne)) {
        std::stringstream ss(ligne);
        std::string nom, gym, badge, champ;
        std::vector<std::string> equipe;

        std::getline(ss, nom, ';');
        std::getline(ss, gym, ';');
        std::getline(ss, badge, ';');

        for (int i = 0; i < 6; ++i) {
            if (!std::getline(ss, champ, ';')) break;
            if (!champ.empty()) equipe.push_back(champ);
        }

        std::string phrase;
        std::getline(ss, phrase);  // Phrase finale ignorée

        if (normaliser(gym) == gymnase_net)
            return Leader(nom, gym, badge, equipe);
    }

    return Leader("Inconnu", gymnase, "", {});
}

// Chargement des Maîtres
std::vector<Maitre> charger_maitres() {
    std::vector<Maitre> maitres;
    std::ifstream fichier(PATH_MAITRES);
    std::string ligne;

    std::getline(fichier, ligne); // Ignorer l'en-tête

    while (std::getline(fichier, ligne)) {
        std::stringstream ss(ligne);
        std::string nom, capacite, champ;
        std::vector<std::string> equipe;

        std::getline(ss, nom, ';');
        std::getline(ss, capacite, ';');

        while (std::getline(ss, champ, ';')) {
            if (!champ.empty()) equipe.push_back(champ);
        }

        maitres.emplace_back(nom, capacite, equipe);
    }

    return maitres;
}