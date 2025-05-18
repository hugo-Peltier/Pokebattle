#include "menu_combat.hpp"
#include "ChargeurCSV.hpp"
#include "combat.hpp"
#include "type_chart.hpp"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <iostream>

using namespace ftxui;

namespace ui {

void menu_combat(Joueur& joueur) {
    std::system("clear");

    std::vector<std::string> options = {
        "Combattre un Gymnase",
        "Affronter un Maître Pokémon",
        "Affronter un Joueur",
        "📊 Voir le tableau des forces/résistances",
        "Retour"
    };

    int selected = 0;
    auto menu = Menu(&options, &selected);
    auto screen = ScreenInteractive::TerminalOutput();

    auto renderer = Renderer(menu, [&] {
        return vbox({
            text("🎮 MODE COMBAT") | bold | center,
            separator(),
            menu->Render() | border,
            separator(),
            text("↑ ↓ pour naviguer, Entrée pour valider") | dim | center
        }) | border;
    });

    bool confirmed = false;
    auto app = CatchEvent(renderer, [&](Event e) {
        if (e == Event::Return) {
            confirmed = true;
            screen.Exit();
            return true;
        }
        return false;
    });

    screen.Loop(app);
    std::system("clear");
    if (!confirmed) return;

    switch (selected) {
        case 0: { // Combattre un Gymnase
            std::vector<std::string> gymnases = {
                "Carmin sur Mer", "Celadopole", "Safrania", "CramoisIle"
            };
            

            int gym_sel = 0;
            auto gym_menu = Menu(&gymnases, &gym_sel);
            auto gym_screen = ScreenInteractive::TerminalOutput();

            auto gym_renderer = Renderer(gym_menu, [&] {
                return vbox({
                    text("Sélectionnez un gymnase") | bold | center,
                    separator(),
                    gym_menu->Render() | border
                }) | border;
            });

            bool gym_ok = false;
            auto gym_app = CatchEvent(gym_renderer, [&](Event e) {
                if (e == Event::Return) {
                    gym_ok = true;
                    gym_screen.Exit();
                    return true;
                }
                return false;
            });

            gym_screen.Loop(gym_app);
            std::system("clear");
            if (!gym_ok) return;

            const std::string& gym = gymnases[gym_sel];
            Leader leader = charger_leader_par_gymnase(gym);
if (leader.getNom() == "Inconnu") {
    std::cerr << "[ERREUR] Aucun leader trouvé pour le gymnase : " << gym << "\n";
    std::cout << "Appuyez sur Entrée pour revenir...\n";
    std::cin.ignore();
    std::cin.get();
    return;
}

auto adversaires = charger_joueurs_par_gymnase(gym);


            lancer_combat_gymnase(joueur, leader, adversaires);
            break;
        }

       case 1: { // Affronter un Maître
            if (joueur.getBadges() < 4) {
                std::cout << "🔒 Il vous faut 4 badges pour affronter un Maître Pokémon.\n";
                std::cin.ignore();
                std::cin.get();
                return;
            }

            auto maitres = charger_maitres();
            std::vector<std::string> noms;
            for (const auto& m : maitres) noms.push_back(m.getNom());

            int sel_maitre = 0;
            auto menu_maitre = Menu(&noms, &sel_maitre);
            auto screen2 = ScreenInteractive::TerminalOutput();

            auto renderer2 = Renderer(menu_maitre, [&] {
                return vbox({
                    text("Choisissez un Maître") | bold | center,
                    separator(),
                    menu_maitre->Render() | border
                });
            });

            bool ok = false;
            auto app2 = CatchEvent(renderer2, [&](Event e) {
                if (e == Event::Return) {
                    ok = true;
                    screen2.Exit();
                    return true;
                }
                return false;
            });

            screen2.Loop(app2);
            std::system("clear");

            if (ok) {
                lancer_combat_maitre(joueur, maitres[sel_maitre]);
            }
            break;
        }
        case 2: { // Affronter un autre joueur (hors gymnase)
            auto joueurs = charger_tous_les_joueurs();  // 🔄 Nouvelle fonction que tu ajoutes dans ChargeurCSV
            std::vector<Joueur> valides;

            for (const auto& j : joueurs) {
                if (!j.getEquipe().empty() && j.getNom() != joueur.getNom()) {
                    valides.push_back(j);
                }
            }

            if (valides.empty()) {
                std::cout << "Aucun joueur libre disponible pour le combat.\n";
                std::cin.ignore();
                std::cin.get();
                return;
            }

            std::vector<std::string> noms;
            for (const auto& j : valides)
                noms.push_back(j.getNom());

            int adv_sel = 0;
            auto adv_menu = Menu(&noms, &adv_sel);
            auto screen3 = ScreenInteractive::TerminalOutput();

            auto renderer3 = Renderer(adv_menu, [&] {
                return vbox({
                    text("Choisissez un joueur à affronter") | bold | center,
                    separator(),
                    adv_menu->Render() | border,
                    separator(),
                    text("↑ ↓ puis Entrée") | dim | center
                }) | border;
            });

            bool valid = false;
            auto app3 = CatchEvent(renderer3, [&](Event e) {
                if (e == Event::Return) {
                    valid = true;
                    screen3.Exit();
                    return true;
                }
                return false;
            });

            screen3.Loop(app3);
            

            if (valid) {
    std::system("clear");
    const auto& adv = valides[adv_sel];
    std::vector<std::string> noms_poke;
    for (const auto& p : adv.getEquipe()) noms_poke.push_back(p.getNom());

    std::cout << "\n🆚 Vous affrontez " << adv.getNom() << " !" << std::endl;

    if (!adv.getPhrase().empty())
        std::cout << "💬 " << adv.getPhrase() << std::endl;

    std::cout << "Appuyez sur Entrée pour commencer le combat...";
    std::cin.ignore();
    std::cin.get();

    lancer_combat_contre(joueur, adv.getNom(), noms_poke);
}

            break;
        }
case 3: {
    std::system("clear");
    afficher_tableau_types();  // ✅ maintenant défini dans type_chart.cpp
    break;
}

        case 4:
        default:
            return;
    }
}

void menu_gymnase(Joueur& joueur) {
    std::system("clear");

    std::vector<std::string> gymnases = {
        "Carmin sur Mer", "Celadopole", "Safrania", "CramoisIle"
    };
    

    int selected = 0;
    auto menu = Menu(&gymnases, &selected);
    auto screen = ScreenInteractive::TerminalOutput();

    auto renderer = Renderer(menu, [&] {
        return vbox({
            text("Infos Gymnase") | bold | center,
            separator(),
            menu->Render() | border,
            separator(),
            text("↑ ↓ puis Entrée") | dim | center
        }) | border;
    });

    bool validated = false;
    auto app = CatchEvent(renderer, [&](Event e) {
        if (e == Event::Return) {
            validated = true;
            screen.Exit();
            return true;
        }
        return false;
    });

    screen.Loop(app);
    std::system("clear");

    if (!validated) return;

    const auto& gym = gymnases[selected];
    auto joueurs = charger_joueurs_par_gymnase(gym);
    auto leader = charger_leader_par_gymnase(gym);

    std::cout << "\n🎯 GYMNASE : " << gym << "\n";
    std::cout << "\n🏆 Leader : " << leader.getNom()
              << " (badge : " << leader.getBadge() << ")\n   Équipe :\n";

    for (const auto& p : leader.getEquipe())
        std::cout << " - " << p.getNom() << "\n";

    std::cout << "\n👤 Joueurs :\n";
    for (const auto& j : joueurs) {
        std::cout << " - " << j.getNom() << "\n";
    }

    std::cout << "\nAppuyez sur Entrée pour revenir...";
    std::cin.ignore();
    std::cin.get();
    std::system("clear");
}

} // namespace ui
