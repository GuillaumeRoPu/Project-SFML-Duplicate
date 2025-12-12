#include <SFML/Graphics.hpp>
#include "InputSystem.h"
#include "Manager/FontManager.h"
#include "Manager/AudioManager.h"
#include "Manager/SceneManager.h"
#include "Manager/LayerManager.h"
#include "Engine/FryEngine.h"
#include "Engine\Time.h"
#include <deque>

#pragma region  INCLUT TES SCENES ICI
#include "Scenes/lascen.h"
#include "Scenes/MainMenu.h"

#pragma endregion


float GetSmoothedFPS();

int main()
{
    // Initialisation du moteur
    fry::Random::Init();

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    // Creation de la fenetre
    sf::RenderWindow window(sf::VideoMode({ 800,800 }), "Speed Hitting : Retro 2D Game", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    // Met l'icône de ton jeu ici !
    sf::Image image;
    if (!image.loadFromFile("res/Images/logo.png"))
    {
        // Error handling...
        std::cerr << "Failed to load icon image!" << std::endl;
        return -1; // or handle gracefully
    }

    window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

    sf::Clock clock;

    FontManager::InitFonts();
    AudioManager::Initialize();

    // Crée tes layers ici
    LayerManager::layers[0] = "Default";
    LayerManager::layers[1] = "Player";
    LayerManager::layers[2] = "Enemy";
    LayerManager::layers[4] = "Bullet";
    LayerManager::layers[8] = "Limit";

    // Build tes scènes ici

    SceneManager::BuildScene<MainMenu>();
    SceneManager::BuildScene<lascen>();

    // Class pour gerer les inputs
    InputSystem input;

    while (window.isOpen())
    {
        float fps = GetSmoothedFPS();

        sf::Text t_fps;
        t_fps.setFont(FontManager::f_pixelify);
        t_fps.setFillColor(sf::Color::White);
        t_fps.setString("FPS : " + std::to_string((int)fps));

        //---------- Events ----------//
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Event si une touche est appuyer
            input.CheckEventInput(event, window);

            // Event si la fenetre est ferme
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //---------- Affichage de la fenetre ----------//

        window.clear(sf::Color::Black);

        SceneManager::PlayCurrentScene(window, input);

        t_fps.setPosition(window.mapPixelToCoords(sf::Vector2i(0, 0)));
        window.draw(t_fps);
        window.display();
    }

    return 0;
}

float GetSmoothedFPS()
{
    static std::deque<float> history;
    static const size_t maxSamples = 100;

    float fps = (Time::UnscaledDeltaTime() > 0) ? 1.0f / Time::UnscaledDeltaTime() : 0.0f;
    history.push_back(fps);
    if (history.size() > maxSamples)
        history.pop_front();

    float sum = 0;
    for (float f : history)
        sum += f;

    return sum / history.size();
}