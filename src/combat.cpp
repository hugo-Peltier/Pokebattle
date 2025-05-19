#include "combat.hpp"
#include "ChargeurCSV.hpp"
#include "config.hpp"
#include "type_chart.hpp"
#include "attaque.hpp"
#include <deque>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <iostream>
#include <algorithm>
#include <sstream>    
#include <chrono>
#include <thread>

using namespace ftxui;


Element afficher_barre_HP(int hp, int max_hp) {
    int total_coeurs = 10;
    float ratio = static_cast<float>(hp) / max_hp;
    float cœurs_reels = ratio * total_coeurs;

    std::string bar;
    for (int i = 0; i < total_coeurs; ++i) {
        if (i + 1 <= cœurs_reels)
            bar += "❤️";       
        else if (i < cœurs_reels)
            bar += "💔";       
        else
            bar += "🖤";     
    }
// @hugop
    return hbox({
        text(bar) | center,
        text(" " + std::to_string(hp) + "/" + std::to_string(max_hp)) | dim
    });
}




int choisir_pokemon(Joueur& joueur, std::vector<bool>& ko_flags, ScreenInteractive& screen) {
    std::vector<std::string> noms;
    bool au_moins_un_disponible = false;

    for (size_t i = 0; i < joueur.getEquipe().size(); ++i) {
        const auto& p = joueur.getEquipe()[i];
        bool ko = ko_flags[i] || p.estKo();
        noms.push_back((ko ? "❌ " : "🔹 ") + p.getNom());

        if (!ko) au_moins_un_disponible = true;
    }

    if (!au_moins_un_disponible) return -1;

    int selected = 0;
    auto menu = Menu(&noms, &selected);
    auto container = Container::Vertical({ menu });

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Choisissez votre Pokémon") | bold | center,
            separator(),
            menu->Render() | border,
            separator(),
            text("↑ ↓ pour naviguer, Entrée pour valider") | dim | center
        }) | border;
    });

    screen.Loop(CatchEvent(renderer, [&](Event e) {
        if (e == Event::Return) {
            if (!ko_flags[selected] && joueur.getEquipe()[selected].getHP() > 0) {
                screen.Exit();
                return true;
            }
        }
        return false;
    }));

    return selected;
}
#include <deque>

#include <deque>

#include <deque>

#include <deque>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

bool combat_simple(Joueur& joueur,
                   Pokemon& adv_poke,
                   std::vector<bool>& ko_flags,
                   int& nb_pokemon_adverse,
                   ScreenInteractive& screen) {
    int idx = choisir_pokemon(joueur, ko_flags, screen);
    if (idx == -1 || ko_flags[idx]) {
    std::cout << "❌ Aucun Pokémon disponible pour combattre !\n";
    screen.Exit();
    return false;
}


    if (idx == -1 || ko_flags[idx]) return false;

    Pokemon& joueur_poke = joueur.getEquipe()[idx];

    bool en_pause = false;
    bool a_quitte = false;
    bool a_change = false;
    bool afficher_menu_attaques = false;

    
    std::string types_joueur = joueur_poke.getTypes()[0];
    if (joueur_poke.getTypes().size() > 1)
        types_joueur += "/" + joueur_poke.getTypes()[1];

    std::string types_adv = adv_poke.getTypes()[0];
    if (adv_poke.getTypes().size() > 1)
        types_adv += "/" + adv_poke.getTypes()[1];

    std::deque<std::string> historique;
    historique.push_back("🔷 Combat : " + joueur_poke.getNom() + " (" + types_joueur + ") VS " + adv_poke.getNom() + " (" + types_adv + ")");

    std::vector<Attaque> attaques_valides;
    for (const auto& atk : joueur_poke.getAttaques()) {
        if (!atk.nom.empty() && atk.puissance > 0)
            attaques_valides.push_back(atk);
    }

    if (attaques_valides.empty()) {
        historique.push_back("❌ Erreur : ce Pokémon n’a aucune attaque valide !");
        en_pause = true;
    }

    std::vector<std::string> options_attaques;
    for (const auto& atk : attaques_valides) {
        options_attaques.push_back("🔸 " + atk.nom + " (" + std::to_string(atk.puissance) + " dmg)");
    }

    int attaque_sel = 0;
    auto menu_attaques = Menu(&options_attaques, &attaque_sel);

    auto bouton_attaquer = Button("Attaquer", [&] {
        if (!attaques_valides.empty())
            afficher_menu_attaques = true;
    });

    auto bouton_retraite = Button("Retraite", [&] {
        a_change = true;
        screen.Exit();
    });

    auto bouton_partir = Button("Partir", [&] {
        a_quitte = true;
        screen.Exit();
    });

    auto bouton_suivant = Button("Suivant", [&] {
        screen.Exit();
    });

    auto attaque_menu_component = CatchEvent(menu_attaques, [&](Event event) {
        if (event == Event::Return) {
            if (attaque_sel >= 0 && attaque_sel < (int)attaques_valides.size()) {
                const auto& attaque = attaques_valides[attaque_sel];

             // @hugop
                float mult = calculerMultiplicateurType(attaque.type, adv_poke.getTypes());
                std::string effet_type;
                if (mult > 1.0f)
                    effet_type = " (C'est super efficace ! Attaque Boostée)";
                else if (mult < 1.0f)
                    effet_type = " (Ce n'est pas très efficace..., son type est résistant)";

                int dmg1 = attaque.puissance * mult;
                adv_poke.recevoirDegats(dmg1);
                historique.push_back(joueur_poke.getNom() + " utilise " + attaque.nom + " : " + std::to_string(dmg1) + " dégâts !" + effet_type);
// @hugop
                if (!adv_poke.estKo()) {
                    const auto& atk_adv = adv_poke.getAttaques().empty() ? Attaque{"Griffes", "Normal", 10} : adv_poke.getAttaques()[0];

                    float mult_adv = calculerMultiplicateurType(atk_adv.type, joueur_poke.getTypes());
                    std::string effet_adv;
                    if (mult_adv > 1.0f)
                        effet_adv = " (C'est super efficace !)";
                    else if (mult_adv < 1.0f)
                        effet_adv = " (Ce n'est pas très efficace...)";

                    int dmg2 = atk_adv.puissance * mult_adv;
                    joueur_poke.recevoirDegats(dmg2);
                    historique.push_back(adv_poke.getNom() + " riposte avec " + atk_adv.nom + " : " + std::to_string(dmg2) + " dégâts." + effet_adv);
                }

                if (joueur_poke.estKo()) {
                    historique.push_back("💀 " + joueur_poke.getNom() + " est K.O.");
                    ko_flags[idx] = true;
                    en_pause = true;
                }

                if (adv_poke.estKo()) {
                    historique.push_back("☠️ " + adv_poke.getNom() + " est K.O.");
                    nb_pokemon_adverse = std::max(0, nb_pokemon_adverse - 1);
                    en_pause = true;
                }

                afficher_menu_attaques = false;
                screen.PostEvent(Event::Custom);
                return true;
            }
        }
        return false;
    });

    auto renderer = Renderer([&] {
        Elements layout;

        layout.push_back(hbox({
            vbox({
                text("👤 " + joueur_poke.getNom()),
                afficher_barre_HP(joueur_poke.getHP(), joueur_poke.getHPInitial())
            }),
            filler(),
            vbox({
                text("🧠 " + adv_poke.getNom()),
                afficher_barre_HP(adv_poke.getHP(), adv_poke.getHPInitial()),
                text("💥 Pokémon restants : " + std::to_string(nb_pokemon_adverse))
            })
        }));

        layout.push_back(separator());

        layout.push_back(vbox({
            text("📜 Historique du combat :") | bold,
            separator(),
            vbox(
                [&] {
                    Elements lignes;
                    int start = std::max(0, (int)historique.size() - 3);
                    for (int i = start; i < (int)historique.size(); ++i)
                        lignes.push_back(text("• " + historique[i]));
                    return lignes;
                }()
            )
        }));

        layout.push_back(separator());

        if (en_pause) {
            layout.push_back(hbox({
                bouton_suivant->Render() | border,
                bouton_partir->Render() | border
            }) | center);
        } else if (afficher_menu_attaques) {
            layout.push_back(text("🎯 Choisissez une attaque :") | center);
            layout.push_back(attaque_menu_component->Render() | border | center);
        } else {
            layout.push_back(hbox({
                bouton_attaquer->Render() | border,
                bouton_retraite->Render() | border,
                bouton_partir->Render() | border
            }) | center);
        }

        return vbox(layout) | border;
    });

    auto container = Container::Vertical({
        bouton_attaquer,
        attaque_menu_component,
        bouton_retraite,
        bouton_partir,
        bouton_suivant
    });

    screen.Loop(Renderer(container, [&] {
        return renderer->Render();
    }));

    if (a_quitte) return false;
    if (a_change) return lancer_combat_contre(joueur, adv_poke.getNom(), {adv_poke.getNom()});
    return !joueur_poke.estKo() && adv_poke.estKo();
}








bool lancer_combat_contre(Joueur& joueur, const std::string& nom_adv, const std::vector<std::string>& equipe_adv_noms) {
    
    std::vector<Pokemon> equipe_adv = trouver_equipe_pokemon(equipe_adv_noms);

    std::cout << "\n🆚 Combat contre " << nom_adv << " 🆚\n";

    bool tous_ko = std::all_of(joueur.getEquipe().begin(), joueur.getEquipe().end(),
                               [](const Pokemon& p) { return p.estKo(); });
    if (tous_ko) {
        std::cout << "❌ Tous vos Pokémon sont déjà K.O. Vous ne pouvez pas combattre.\n";
        std::cin.ignore();
        std::cin.get();
        return false;
    }
// @hugop
    std::vector<bool> ko_joueur(joueur.getEquipe().size(), false);
    std::vector<bool> ko_adverse(equipe_adv.size(), false);
    int nb_pokemon_adverse = equipe_adv.size();

    ScreenInteractive screen = ScreenInteractive::TerminalOutput();

    while (true) {
        if (std::all_of(ko_adverse.begin(), ko_adverse.end(), [](bool b) { return b; })) {
            std::cout << "\n✅ Victoire contre " << nom_adv << " !\n";
            joueur.ajouterVictoire();
            std::cout << "Appuyez sur Entrée pour continuer...\n";
            std::cin.ignore();
            std::cin.get();
            return true;
        }

        if (std::all_of(ko_joueur.begin(), ko_joueur.end(), [](bool b) { return b; })) {
            std::cout << "\n❌ Tous vos Pokémon sont K.O.\n";
            joueur.ajouterDefaite(); 
            std::cout << "Appuyez sur Entrée pour continuer...\n";
            std::cin.ignore();
            std::cin.get();
            return false;
        }

        size_t idx_adv = 0;
        while (idx_adv < equipe_adv.size() && ko_adverse[idx_adv])
            ++idx_adv;

        if (idx_adv >= equipe_adv.size()) continue;

     // @hugop
        bool vainqueur = combat_simple(joueur, equipe_adv[idx_adv], ko_joueur, nb_pokemon_adverse, screen);
        if (vainqueur)
            ko_adverse[idx_adv] = true;
        else if (std::all_of(ko_joueur.begin(), ko_joueur.end(), [](bool b) { return !b; })) {
            std::cout << "🔙 Vous quittez le combat.\n";
            joueur.ajouterDefaite();
            return false;
        }
    }
}

// @hugop
bool demander_confirmation(const std::string& message) {
    ScreenInteractive screen = ScreenInteractive::TerminalOutput();
    int selected = 0;
    std::vector<std::string> choix = { "Oui", "Non" };

    auto menu = Menu(&choix, &selected);
    auto renderer = Renderer([&] {
        return vbox({
            text(message) | bold | center,
            separator(),
            menu->Render() | border
        });
    });
// @hugop
    screen.Loop(CatchEvent(renderer, [&](Event e) {
        if (e == Event::Return) {
            screen.Exit();
            return true;
        }
        return false;
    }));

    return selected == 0;
}

void lancer_combat_gymnase(Joueur& joueur, const Leader& leader, const std::vector<Joueur>& adversaires) {
    for (size_t i = 0; i < adversaires.size(); ++i) {
        const auto& adv = adversaires[i];
        std::vector<std::string> noms;
        for (const auto& p : adv.getEquipe()) noms.push_back(p.getNom());

        if (!lancer_combat_contre(joueur, adv.getNom(), noms)) return;

        if (i < adversaires.size() - 1 && !demander_confirmation("Souhaitez-vous affronter le prochain adversaire ?")) {
            std::cout << "🏃‍ Vous quittez le gymnase.\n";
            return;
        }
    }

    std::vector<std::string> noms_leader;
    for (const auto& p : leader.getEquipe()) noms_leader.push_back(p.getNom());

    if (!lancer_combat_contre(joueur, leader.getNom(), noms_leader)) return;

    std::cout << "🎖 Vous avez vaincu le leader " << leader.getNom()
              << " ! Vous recevez le badge " << leader.getBadge() << ".\n";
    joueur.ajouterBadge();
    std::cout << "Appuyez sur Entrée pour continuer...\n";
    std::cin.ignore();
    std::cin.get();
}

void lancer_combat_maitre(Joueur& joueur, const Maitre& maitre) {
    std::vector<std::string> noms;
    for (const auto& p : maitre.getEquipe()) noms.push_back(p.getNom());

    std::cout << "\n👑 Vous affrontez le Maître Pokémon " << maitre.getNom() << " !\n";
    lancer_combat_contre(joueur, maitre.getNom(), noms);
    std::system("clear");

    
    std::vector<int> verts = { 120, 34, 28, 22, 28, 34 };

    std::string ascii_victoire = R"(
██╗   ██╗██╗ ██████╗████████╗ ██████╗ ██╗██████╗ ███████╗    ██╗
██║   ██║██║██╔════╝╚══██╔══╝██╔═══██╗██║██╔══██╗██╔════╝    ██║
██║   ██║██║██║        ██║   ██║   ██║██║██████╔╝█████╗      ██║
╚██╗ ██╔╝██║██║        ██║   ██║   ██║██║██╔══██╗██╔══╝      ╚═╝
 ╚████╔╝ ██║╚██████╗   ██║   ╚██████╔╝██║██║  ██║███████╗    ██╗
  ╚═══╝  ╚═╝ ╚═════╝   ╚═╝    ╚═════╝ ╚═╝╚═╝  ╚═╝╚══════╝    ╚═╝
)";

    std::istringstream stream(ascii_victoire);
    std::string ligne;
    int ligne_num = 0;

    while (std::getline(stream, ligne)) {
        int code_vert = verts[ligne_num % verts.size()];
        int pad = std::max(0, (int)(80 - ligne.size()) / 2);
        std::cout << "\033[38;5;" << code_vert << "m"
                  << std::string(pad, ' ') << ligne
                  << "\033[0m\n";
        ++ligne_num;
    }

    std::string message = "🎉 Félicitations, vous avez vaincu le Maître Pokémon " + maitre.getNom() + " !";
    int pad_msg = std::max(0, (int)(80 - message.size()) / 2);
    std::cout << "\n" << std::string(pad_msg, ' ');

    for (char c : message) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::string suite = "Appuyez sur Entrée pour continuer...";
    int pad_suite = std::max(0, (int)(80 - suite.size()) / 2);
    std::cout << "\n\n" << std::string(pad_suite, ' ') << suite << std::endl;

    std::cin.ignore();
    std::cin.get();
}

// @hugop