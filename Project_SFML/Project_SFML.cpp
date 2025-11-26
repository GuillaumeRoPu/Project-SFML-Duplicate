#include <SFML/Graphics.hpp>
#include "InputSystem.h"
#include "Manager/FontManager.h"
#include "Manager/SceneManager.h"
#include "Engine/FryEngine.h"
#include <deque>

#pragma region  INCLUT TES SCENES ICI
#include "Scenes/lascen.h"

#pragma endregion


float GetSmoothedFPS();

int main()
{

    // Initialisation du moteur
    fry::Random::Init();

    // Creation de la fenetre
    sf::RenderWindow window(sf::VideoMode({ 800,800 }), "2D Game");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    // Met l'icône de ton jeu ici !
    //window.setIcon(sf::Image(""));

    sf::Clock clock;

    FontManager::InitFonts();

    // Build tes scènes ici
    SceneManager::BuildScene<lascen>();

    // Class pour gerer les inputs
    InputSystem input;

    while (window.isOpen())
    {
        fry::deltaTime = clock.restart().asSeconds();
        fry::fps = GetSmoothedFPS();

        sf::Text fps;
        fps.setFont(FontManager::f_pixelify);

        fps.setFillColor(sf::Color::Black);
        fps.setString("FPS : " + std::to_string((int)fry::fps));

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

        fps.setPosition(window.mapPixelToCoords(sf::Vector2i(0, 0)));
        window.draw(fps);
        window.display();
    }

    return 0;
}

float GetSmoothedFPS()
{
    static std::deque<float> history;
    static const size_t maxSamples = 100;

    float fps = (fry::deltaTime > 0) ? 1.0f / fry::deltaTime : 0.0f;
    history.push_back(fps);
    if (history.size() > maxSamples)
        history.pop_front();

    float sum = 0;
    for (float f : history)
        sum += f;

    return sum / history.size();
}