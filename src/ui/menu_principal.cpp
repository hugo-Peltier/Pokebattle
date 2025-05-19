#include "menu_principal.hpp"
#include "menu_combat.hpp"
#include "ChargeurCSV.hpp"
#include "combat.hpp"
#include "config.hpp"
#include "credits.hpp"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <SFML/Audio.hpp>  
#include <iostream>
#include <fstream>
#include <algorithm>

sf::SoundBuffer buffer_menu;
sf::Sound sound_menu;
bool son_menu_charge = false;

using namespace ftxui;

namespace ui {

void sauvegarderJoueurTemporaire(const Joueur& joueur) {
    std::ofstream fichier(PATH_JOUEURS, std::ios::app);
    if (!fichier.is_open()) {
        std::cerr << "[ERREUR] Impossible d'ouvrir le fichier joueur.csv\n";
        return;
    }

    fichier << joueur.getNom() << "," << "SESSION" << "," << joueur.getBadges()
            << "," << joueur.getVictoires() << "," << joueur.getDefaites();

    for (const auto& p : joueur.getEquipe()) {
        fichier << "," << p.getNom();
    }

    fichier << "\n";
    std::cout << "💾 " << joueur.getNom() << " sauvegardé dans joueur.csv\n";
}

void supprimerJoueurTemporaire(const Joueur& joueur) {
    std::ifstream in(PATH_JOUEURS);
    std::ofstream out("temp.csv");
    std::string ligne;

    while (std::getline(in, ligne)) {
        if (ligne.find(joueur.getNom() + ",SESSION") != 0) {
            out << ligne << "\n";
        }
    }

    in.close();
    out.close();

    std::remove(PATH_JOUEURS.c_str());
    std::rename("temp.csv", PATH_JOUEURS.c_str());

    std::cout << "🧹 " << joueur.getNom() << " supprimé de joueur.csv\n";
}

void menu_principal(Joueur& joueur) {
    auto pokemons = charger_pokemons(PATH_POKEMON);
    bool quitter = false;
    bool equipe_creee = !joueur.getEquipe().empty();

    static sf::SoundBuffer buffer_menu;
static sf::Sound sound_menu;

bool audio_menu_charge = false;

if (!MODE_SILENCIEUX) {
    if (!buffer_menu.loadFromFile("/mnt/cppProject/Pokebattle/audio/menu.wav")) {
        std::cerr << "[AUDIO] menu.wav non lisible\n";
        MODE_SILENCIEUX = true;
    } else {
        sound_menu.setBuffer(buffer_menu);
        sound_menu.setLoop(true);
        sound_menu.play();
        audio_menu_charge = true;
    }
}


    

    while (!quitter) {
        std::system("clear");

        auto screen = ScreenInteractive::TerminalOutput();
        int selected = 0;
        std::vector<std::string> options = {
    "Créer mon équipe Pokémon",
    "Afficher mes Pokémon",
    "Modifier mon équipe Pokémon",  
    "Mode Combat",
    "Afficher mes statistiques",
    "Soigner mes Pokémon",
    "Crédits / Licence",
    "Quitter"
};


        auto menu = Menu(&options, &selected);

        auto renderer = Renderer(menu, [&] {
            return vbox({
                text("██████   ██████  ██   ██ ███████ ██████   █████  ████████ ████████ ██      ███████ ") | bold | color(Color::Cyan) | center,
                text("██   ██ ██    ██ ██  ██  ██      ██   ██ ██   ██    ██       ██    ██      ██      ") | bold | color(Color::Blue) | center,
                text("██████  ██    ██ █████   █████   ██████  ███████    ██       ██    ██      █████   ") | bold | color(Color::Green) | center,
                text("██      ██    ██ ██  ██  ██      ██   ██ ██   ██    ██       ██    ██      ██      ") | bold | color(Color::Magenta) | center,
                text("██       ██████  ██   ██ ███████ ██████  ██   ██    ██       ██    ███████ ███████ ") | bold | color(Color::Red) | center,
                separator(),
                text("Menu Principal") | bold | center,
                menu->Render() | border,
                separator(),
                text("Utilisez ↑ ↓ puis Entrée pour sélectionner") | dim | center
            });
        });
        

        bool validated = false;
        auto app = CatchEvent(renderer, [&](Event event) {
            if (event == Event::Return) {
                validated = true;
                screen.Exit();
                return true;
            }
            return false;
        });

        screen.Loop(app);
        std::system("clear");

        if (validated) {
            switch (selected) {
                   case 0: {
    if (equipe_creee) {
        std::cout << "⚠️ Équipe déjà créée !\n";
        std::cin.ignore();
        std::cin.get();
        break;  
    }
    bool terminer_creation = false;

    while (!terminer_creation) {
    auto screen2 = ScreenInteractive::TerminalOutput();
        int selected_poke = 0;

        std::vector<std::string> options_poke;

        options_poke.push_back("✅ J’ai fini ma sélection");
        for (const auto& p : pokemons) {
            bool deja_choisi = std::any_of(joueur.getEquipe().begin(), joueur.getEquipe().end(),
                                           [&](const Pokemon& pk) { return pk.getNom() == p.getNom(); });
            options_poke.push_back((deja_choisi ? "✅ " : "🔹 ") + p.getNom());
        }
        options_poke.push_back("❌ Annuler et revenir");

    auto menu_poke = Menu(&options_poke, &selected_poke);
    auto renderer = Renderer(menu_poke, [&] {
            return vbox({
                text("🎯 Sélectionnez vos Pokémon (1 à 6)") | bold | center,
                separator(),
                menu_poke->Render() | frame | vscroll_indicator | border,
                separator(),
                text("↑ ↓ puis Entrée pour valider ou sélectionner") | dim | center,
                text("Pokémon sélectionnés : " + std::to_string(joueur.getEquipe().size()) + "/6") | bold | center
            });
        });

    bool choisi = false;
    auto app = CatchEvent(renderer, [&](Event e) {
        if (e == Event::Return) {
            if (selected_poke == 0) { 
                    if (joueur.getEquipe().size() >= 1) {
                        terminer_creation = true;
                    } else {
                        std::cout << "⚠️ Sélectionnez au moins 1 Pokémon pour continuer.\n";
                        std::cin.ignore();
                        std::cin.get();
                    }
            } 



else if (selected_poke == options_poke.size() - 1) { 
                    std::cout << "❌ Création annulée. Retour au menu principal.\n";
    std::cin.ignore();
    std::cin.get();
    joueur.getEquipe().clear();      
    terminer_creation = true;       
    screen2.Exit();                 
                    return true;




// @hugop

                } else {
                    const auto& choix = pokemons[selected_poke - 1];
                    bool deja_choisi = std::any_of(joueur.getEquipe().begin(), joueur.getEquipe().end(),
                                                   [&](const Pokemon& p) { return p.getNom() == choix.getNom(); });

                    if (!deja_choisi && joueur.getEquipe().size() < 6) {
                        joueur.ajouterPokemon(choix);
                    }

                
                if (joueur.getEquipe().size() == 6) {
                    terminer_creation = true;
                }
            }

            choisi = true;
            screen2.Exit();
            return true;
        }
            return false;
        });

        screen2.Loop(app);
    if (choisi) std::system("clear");
    }


        

    sauvegarderJoueurTemporaire(joueur);
    equipe_creee = true;
    break;
}

               case 1: {
    if (joueur.getEquipe().empty()) {
        std::cout << "❌ Vous n'avez pas encore créé votre équipe !\n";
        std::cin.ignore();
        std::cin.get();
        break;
    }

    std::cout << "\n📋 Ton équipe actuelle :\n";

    for (const auto& p : joueur.getEquipe()) {
        std::cout << "🔹 " << p.getNom() << " (" << p.getTypes()[0];
        if (p.getTypes().size() > 1)
            std::cout << "/" << p.getTypes()[1];
        std::cout << ") - HP : " << p.getHP() << " / " << p.getHPInitial();

        if (!p.getAttaques().empty()) {
            std::cout << " - Attaques : ";
            for (size_t i = 0; i < p.getAttaques().size(); ++i) {
                std::cout << p.getAttaques()[i].nom << " (" << p.getAttaques()[i].puissance << " dmg)";
                if (i < p.getAttaques().size() - 1)
                    std::cout << " / ";
            }
        }

        std::cout << "\n";  
    }

    std::cout << "\nAppuyez sur Entrée pour continuer...\n";
    std::cin.ignore();
    std::cin.get();
    break;
}

                case 3: {
    if (joueur.getEquipe().size() < 1) {
        std::cout << "⚠️ Créez une équipe avec au moins 1 Pokémon pour entrer en combat.\n";
        std::cin.ignore();
        std::cin.get();
        break;
    }
    menu_combat(joueur);
    break;
}

    case 2: {
    if (joueur.getEquipe().empty()) {
        std::cout << "❌ Vous n'avez pas encore d'équipe à modifier.\n";
        std::cin.ignore();
        std::cin.get();
        break;
    }

    
    std::vector<Pokemon> equipe_originale = joueur.getEquipe();

    bool terminer_modif = false;

    while (!terminer_modif) {
        auto screen2 = ScreenInteractive::TerminalOutput();
        int selected = 0;

        std::vector<std::string> options_modif = {
            "✅ Valider mon équipe",
            "➕ Ajouter un Pokémon",
            "➖ Retirer un Pokémon",
            "❌ Annuler les modifications"
        };

        auto menu = Menu(&options_modif, &selected);
        auto renderer = Renderer(menu, [&] {
            return vbox({
                text("🛠 Modifier votre équipe Pokémon") | bold | center,
                separator(),
                menu->Render() | frame | vscroll_indicator | border,
                separator(),
                text("Pokémon actuels : " + std::to_string(joueur.getEquipe().size()) + "/6") | center
            });
        });

        bool action = false;
        auto app = CatchEvent(renderer, [&](Event e) {
            if (e == Event::Return) {
                screen2.Exit();
                action = true;
                return true;
            }
            return false;
        });

        screen2.Loop(app);
        std::system("clear");
        if (!action) continue;

        if (selected == 0) { 
            if (joueur.getEquipe().size() >= 1) {
                sauvegarderJoueurTemporaire(joueur);
                std::cout << "💾 Équipe sauvegardée avec succès.\n";
                std::cin.ignore();
                std::cin.get();
                terminer_modif = true;
            } else {
                std::cout << "⚠️ Vous devez avoir au moins 1 Pokémon.\n";
                std::cin.ignore();
                std::cin.get();
            }
        }
        else if (selected == 1) { 
            auto all = charger_pokemons(PATH_POKEMON);
            std::vector<std::string> noms;
            std::vector<const Pokemon*> options_pokemon;

            for (const auto& p : all) {
                bool deja = std::any_of(joueur.getEquipe().begin(), joueur.getEquipe().end(),
                                        [&](const Pokemon& pk) { return pk.getNom() == p.getNom(); });
                if (!deja) {
                    noms.push_back("🔹 " + p.getNom());
                    options_pokemon.push_back(&p);
                }
            }

            if (noms.empty() || joueur.getEquipe().size() >= 6) {
                std::cout << "⚠️ Impossible d'ajouter plus de Pokémon.\n";
                std::cin.ignore();
                std::cin.get();
                continue;
            }

            int choix = 0;
            auto menu_add = Menu(&noms, &choix);
            auto screen_add = ScreenInteractive::TerminalOutput();

            auto renderer_add = Renderer(menu_add, [&] {
    return vbox({
        text("Sélectionnez un Pokémon à ajouter") | bold | center,
        separator(),
        menu_add->Render() | frame | vscroll_indicator | border,
        separator(),
        text("↑ ↓ puis Entrée pour valider") | dim | center
    });
});


            bool valid = false;
            screen_add.Loop(CatchEvent(renderer_add, [&](Event e) {
                if (e == Event::Return) {
                    valid = true;
                    screen_add.Exit();
                    return true;
                }
                return false;
            }));

            if (valid) {
                const Pokemon* poke_choisi = options_pokemon[choix];
                joueur.ajouterPokemon(*poke_choisi);
                std::cout << "✅ " << poke_choisi->getNom() << " ajouté à votre équipe.\n";
                std::cin.ignore();
                std::cin.get();
            }
        }
        else if (selected == 2) { // ➖ Retirer
            if (joueur.getEquipe().empty()) {
                std::cout << "⚠️ Aucun Pokémon à retirer.\n";
                std::cin.ignore();
                std::cin.get();
                continue;
            }

            std::vector<std::string> noms;
            for (const auto& p : joueur.getEquipe())
                noms.push_back("🗑 " + p.getNom());

            int choix = 0;
            auto menu_del = Menu(&noms, &choix);
            auto screen_del = ScreenInteractive::TerminalOutput();

            auto renderer_del = Renderer(menu_del, [&] {
                return vbox({
                    text("Sélectionnez un Pokémon à retirer") | bold | center,
                    separator(),
                    menu_del->Render() | border
                });
            });
// @hugop
            bool valid = false;
            screen_del.Loop(CatchEvent(renderer_del, [&](Event e) {
                if (e == Event::Return) {
                    valid = true;
                    screen_del.Exit();
                    return true;
                }
                return false;
            }));

            if (valid) {
                joueur.getEquipe().erase(joueur.getEquipe().begin() + choix);
                std::cout << "🗑 Pokémon retiré.\n";
                std::cin.ignore();
                std::cin.get();
            }
        }
        else if (selected == 3) { // ❌ Annuler
            joueur.getEquipe() = equipe_originale;  // 🔁 Restauration
            std::cout << "🛑 Modifications annulées. Équipe restaurée.\n";
            std::cin.ignore();
            std::cin.get();
            terminer_modif = true;
        }
    }
// @hugop
    break;
}



                case 4:
                    joueur.afficherStatistiques();
                    std::cout << "\nAppuyez sur Entrée pour continuer...\n";
                    std::cin.ignore();
                    std::cin.get();
                    break;

                case 5: {
                    if (joueur.getEquipe().empty()) {
                        std::cout << "❌ Vous n'avez pas encore d'équipe à soigner !\n";
                    } else {
                        bool tous_full = std::all_of(joueur.getEquipe().begin(), joueur.getEquipe().end(),
                        [](const Pokemon& p) { return p.getHP() == p.getHPInitial(); });

                        if (tous_full) {
                            std::cout << "✅ Tous vos Pokémon sont déjà en pleine forme !\n";
                        } else {
                            joueur.soignerEquipe();
                            std::cout << "🩺 Tous vos Pokémon ont été soignés !\n";
                        }
                    }

                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
case 6: {
    ui::afficher_credits();
    break;
}

                case 7:
                    supprimerJoueurTemporaire(joueur);
                    quitter = true;
                    std::cout << "\n👋 Merci d'avoir joué à PokéBattle👋 N'hésitez pas à revenir nous voir !\n";
                    std::cout << "Et sourtout, n'hésitez pas à rejouer, nous vous attendons pour vos prochaines\n";
                    break;
            }
        }
    }

        if (audio_menu_charge && sound_menu.getStatus() == sf::Sound::Playing) {
        sound_menu.stop();
    }

} 
} 

