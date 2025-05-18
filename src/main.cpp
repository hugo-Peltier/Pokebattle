#include "Joueur.hpp"
#include "intro.hpp"
#include "config.hpp"

#include <iostream>
#include <string>
#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>

using namespace ftxui;

bool initialiser_audio() {
    std::cout << "🔊 Initialisation audio... ";
    std::cout.flush();

    try {
        sf::SoundBuffer buffer_test;
        if (buffer_test.loadFromFile("/mnt/cppProject/Pokebattle/audio/intro.wav")) {
            sf::Sound sound_test;
            sound_test.setBuffer(buffer_test);
            sound_test.play();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            std::cout << "✅ périphérique audio détecté.\n";
            return true;
        } else {
            std::cerr << "\n[AUDIO] Fichier audio introuvable.\n";
        }
    } catch (...) {
        std::cerr << "\n[AUDIO] Erreur SFML/ALSA inattendue.\n";
    }

    std::cout << "❌ Aucun périphérique audio détecté. Passage en mode silencieux.\n";
    return false;
}

bool demander_mode_audio() {
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> choix = {
        "🔊 Oui, activer le son",
        "🔇 Non, rester silencieux"
    };
    int selected = 0;

    auto menu = Menu(&choix, &selected);

    auto renderer = Renderer(menu, [&] {
        return vbox({
            text("🎧 Souhaitez-vous activer le son ?") | bold | center,
            separator(),
            menu->Render() | border | center,
            separator(),
            text("Utilisez ↑ ↓ puis Entrée pour choisir") | dim | center
        }) | center;
    });

    auto control = CatchEvent(renderer, [&](Event e) {
        if (e == Event::Return) {
            screen.Exit();
            return true;
        }
        return false;
    });

    screen.Loop(control);
    return selected == 0;  // true si "Oui"
}

int main() {
    std::cout << "🎮 Bienvenue dans PokéBattle\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    // 🔍 Poser la question à l'utilisateur
    bool audio_voulu = demander_mode_audio();

    // 🔧 Initialisation audio si demandé
    if (audio_voulu) {
        MODE_SILENCIEUX = !initialiser_audio();  // false si OK
    } else {
        MODE_SILENCIEUX = true;
    }

    std::string nom;
    std::cout << "📝 Entrez votre nom de joueur : ";
    std::getline(std::cin, nom);

    Joueur joueur(nom);
    ui::afficher_intro(joueur);

    return 0;
}
