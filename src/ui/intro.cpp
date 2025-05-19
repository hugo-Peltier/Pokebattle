#include "intro.hpp"
#include "menu_principal.hpp"
#include "config.hpp"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <SFML/Audio.hpp>
#include <iostream>
#include<tutoriel.hpp>

using namespace ftxui;
using namespace std::chrono_literals;

namespace ui {

void afficher_intro(Joueur& joueur) {
    auto screen = ScreenInteractive::TerminalOutput();

    int progress = 0;
    std::vector<std::string> symboles_types = {
        "🔥", "🌿", "💧", "⚡", "👻", "🪨", "🐉", "🧠", "🧊"
    };

    sf::SoundBuffer buffer_intro;
sf::Sound sound_intro;
bool audio_charge = false;

if (!MODE_SILENCIEUX) {
    if (buffer_intro.loadFromFile("/mnt/cppProject/Pokebattle/audio/intro.wav")) {
        sound_intro.setBuffer(buffer_intro);
        sound_intro.setLoop(false);
        sound_intro.play();
        audio_charge = true;
    } else {
        std::cerr << "[AUDIO] intro.wav non lisible\n";
    }
}



    auto renderer = Renderer([&] {
        int nb_symboles = progress / 4;
        std::string ligne_symboles;
        for (int i = 0; i < nb_symboles; ++i)
            ligne_symboles += symboles_types[i % symboles_types.size()];

        return vbox({
            text("██████╗  ██████╗ ██╗  ██╗███████╗██████╗  █████╗ ████████╗████████╗██╗     ███████╗") | bold | color(Color::Red),
            text("██╔══██╗██╔═══██╗██║ ██╔╝██╔════╝██╔══██╗██╔══██╗╚══██╔══╝╚══██╔══╝██║     ██╔════╝") | bold | color(Color::Yellow),
            text("██████╔╝██║   ██║█████╔╝ █████╗  ██████╔╝███████║   ██║      ██║   ██║     █████╗  ") | bold | color(Color::Green),
            text("██╔═══╝ ██║   ██║██╔═██╗ ██╔══╝  ██╔══██╗██╔══██║   ██║      ██║   ██║     ██╔══╝  ") | bold | color(Color::Blue),
            text("██║     ╚██████╔╝██║  ██╗███████╗██████╔╝██║  ██║   ██║      ██║   ███████╗███████╗") | bold | color(Color::Cyan),
            text("╚═╝      ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═════╝ ╚═╝  ╚═╝   ╚═╝      ╚═╝   ╚══════╝╚══════╝") | bold | color(Color::White),
            separator(),
            text("Bienvenue dans PokéBattle!") | bold | center,
            separator(),
            text("Chargement...") | center,
            text("[" + ligne_symboles + std::string(25 - nb_symboles, ' ') + "]") | color(Color::Green) | center,
            text(std::to_string(progress) + " %") | center
        }) | border | center;
    });

    auto component = CatchEvent(renderer, [](Event) { return false; });

    std::thread animation([&] {
        for (int i = 0; i <= 100; ++i) {
            progress = i;
            screen.PostEvent(Event::Custom);
            std::this_thread::sleep_for(60ms);
        }
        screen.Exit();
    });

    screen.Loop(component);
    animation.join();

    if (audio_charge && sound_intro.getStatus() == sf::Sound::Playing) {
        sound_intro.stop();
    }
    
    

    std::system("clear");
    ui::afficher_tutoriel(joueur);
    std::system("clear");
    menu_principal(joueur);
}
// @hugop
} 
