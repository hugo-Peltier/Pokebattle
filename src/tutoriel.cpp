#include "tutoriel.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>
#include <string>

using namespace ftxui;

namespace ui {

void afficher_tutoriel(Joueur& joueur) {
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> lignes = {
         "📘 TUTORIEL POKÉBATTLE",
        "",
        "Bienvenue dans PokéBattle, une simulation de combat Pokémon entièrement en C++.",
        "Vous allez affronter des joueurs, leaders et maîtres dans un système de combat par tours.",
        "",
        "🔊 Note : Ce jeu fonctionne actuellement en mode SILENCIEUX.",
        "Cela peut être dû à une incompatibilité audio (SFML) ou un protocole audio manquant.",
        "",
        "🎮 CONTROLES GÉNÉRAUX",
        " - ↑ et ↓ : naviguer dans les menus",
        " - Entrée : valider un choix",
        " - Échap : revenir ou quitter un menu",
        "ℹ️ Conseil : Si les flèches ne fonctionnent pas bien, cliquez dans la fenêtre du terminal",
        "   puis appuyez à nouveau sur Entrée.",
        "",
        "🐾 CONSTITUER VOTRE ÉQUIPE",
        " - Choisissez entre 1 et 6 Pokémon.",
        " - Chaque Pokémon a :",
        "   ➤ 1 ou 2 types (ex: Feu, Eau, Plante...)",
        "   ➤ Des HP (points de vie)",
        "   ➤ 2 attaques, avec type et puissance",
        "",
        "⚔️ COMBAT TOUR PAR TOUR",
        " - Le joueur attaque toujours en premier.",
        " - Chaque tour, vous choisissez :",
        "   🔘 Attaquer : sélectionnez une attaque",
        "   🔁 Retraite : changez de Pokémon",
        "   ❌ Partir : fuyez le combat (vous perdez)",
        "",
        "📊 TYPES ET FAIBLESSES",
        " - Certaines attaques sont super efficaces (×2) ou peu efficaces (×0.5).",
        " - Cela dépend du type de l’attaque VS les types du Pokémon adverse.",
        "",
        "🧪 EXEMPLES :",
        " - Pistolet à O (type Eau) sur Salamèche (type Feu) → ×2",
        " - Flammèche (Feu) sur Carapuce (Eau) → ×0.5",
        "",
        "🏛 AFFRONTER UN GYMNASE",
        " - Un gymnase contient plusieurs joueurs + 1 Leader final.",
        " - Battez tous les joueurs pour débloquer le Leader.",
        " - Si vous gagnez, vous remportez son BADGE.",
        "",
        "👑 AFFRONTER UN MAÎTRE",
        " - Nécessite 4 badges.",
        " - Les maîtres ont une CAPACITÉ SPÉCIALE : +25% dégâts.",
        "",
        "📈 STATISTIQUES ET MENU",
        " - Soigner vos Pokémon (HP restaurés)",
        " - Afficher vos stats (victoires, défaites, badges)",
        " - Modifier ou consulter votre équipe",
        "",
        "💬 INTERACTIONS",
        " - Parlez à vos Pokémon (message unique par espèce)",
        " - Parlez aux entraîneurs que vous avez vaincus",
        "",
        "🎯 Objectif : vaincre tous les gymnases, battre les Maîtres et devenir le meilleur dresseur !",
        "",
         "@PELTIER Hugo & GLUCINA Candice / 2025-ESILV-Project C++ / +33783513028",
        "",
        "Appuyez sur Entrée pour faire défiler, ou Échap pour passer le tutoriel."
    };

    int selected = 0;
    auto menu = Menu(&lignes, &selected);

    auto renderer = Renderer(menu, [&] {
        return vbox({
            // Logo ASCII centré ligne par ligne avec couleur rouge dégradé
            text("████████╗██╗   ██╗████████╗ ██████╗ ██████╗ ██╗███████╗██╗     ") | bold | center | color(Color::Red),
            text("╚══██╔══╝██║   ██║╚══██╔══╝██╔═══██╗██╔══██╗██║██╔════╝██║     ") | bold | center | color(Color::RGB(255, 64, 64)),
            text("   ██║   ██║   ██║   ██║   ██║   ██║██████╔╝██║█████╗  ██║     ") | bold | center | color(Color::RGB(255, 100, 100)),
            text("   ██║   ██║   ██║   ██║   ██║   ██║██╔══██╗██║██╔══╝  ██║     ") | bold | center | color(Color::RGB(255, 140, 140)),
            text("   ██║   ╚██████╔╝   ██║   ╚██████╔╝██║  ██║██║███████╗███████╗") | bold | center | color(Color::RGB(255, 180, 180)),
            text("   ╚═╝    ╚═════╝    ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚═╝╚══════╝╚══════╝") | bold | center | color(Color::RGB(255, 220, 220)),
            separator(),
            hbox({
                text("📘 ") | color(Color::RGB(100, 150, 255)),
                text("Tutoriel de PokéBattle") | bold | color(Color::Yellow)
            }) | center,
            separator(),
            menu->Render() | frame | vscroll_indicator | border,
            separator(),
            text("↑ ↓ pour défiler — Entrée ou Échap pour quitter") | dim | center
        }) | center;
    });

    auto control = CatchEvent(renderer, [&](Event e) {
        if (e == Event::Return || e == Event::Escape) {
            screen.Exit();
            return true;
        }
        return false;
    });

    screen.Loop(control);
    std::system("clear");
}

} // namespace ui
