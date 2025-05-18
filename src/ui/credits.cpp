#include "credits.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <thread>
#include <chrono>

namespace ui {
    void afficher_credits() {
        using namespace ftxui;

        auto screen = ScreenInteractive::TerminalOutput();

        std::vector<std::string> lignes = {
            "🔹 Projet PokéBattle - Simulation Pokémon en C++ 🔹",
            "",
            "Développé par :",
            "PELTIER Hugo",
            "GLUCINA Candice",
            "",
            "Licence : MIT",
            "",
            "Permission est accordée, gratuitement, à toute personne obtenant",
            "une copie de ce logiciel et des fichiers de documentation associés",
            "de traiter le Logiciel sans restriction, y compris sans limitation",
            "les droits d'utiliser, copier, modifier, fusionner, publier, distribuer,",
            "sous-licencier et/ou vendre des copies du Logiciel.",
            "",
            "LE LOGICIEL EST FOURNI \"EN L'ÉTAT\",",
            "SANS GARANTIE D'AUCUNE SORTE.",
            "",
            "Appuyez sur Entrée pour revenir."
        };

        int index = 0;

        auto renderer = Renderer([&] {
            Elements affichage;
            for (int i = 0; i < index && i < (int)lignes.size(); ++i)
                affichage.push_back(text(lignes[i]) | center);
            return vbox({
                vbox(std::move(affichage)) | border | center
            });
        });

        auto app = CatchEvent(renderer, [&](Event e) {
            if (e == Event::Return && index >= (int)lignes.size()) {
                screen.Exit();
                return true;
            }
            return false;
        });

        std::thread animate([&] {
            while (index < (int)lignes.size()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                index++;
                screen.PostEvent(Event::Custom);
            }
        });

        screen.Loop(app);
        animate.join();
        std::system("clear");
    }
}
