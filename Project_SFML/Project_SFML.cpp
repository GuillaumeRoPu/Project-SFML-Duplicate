#include <SFML/Graphics.hpp>
#include "InputSystem.h"
#include "Manager/FontManager.h"
#include "Manager/SceneManager.h"
#include "Engine/FryEngine.h"
#include "Engine\Time.h"
#include <deque>

#pragma region  INCLUT TES SCENES ICI

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
    

    // Class pour gerer les inputs
    InputSystem input;

    while (window.isOpen())
    {
        float fps = GetSmoothedFPS();

        sf::Text t_fps;
        t_fps.setFont(FontManager::f_pixelify);
        t_fps.setFillColor(sf::Color::Black);
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

    float fps = (Time::DeltaTime() > 0) ? 1.0f / Time::DeltaTime() : 0.0f;
    history.push_back(fps);
    if (history.size() > maxSamples)
        history.pop_front();

    float sum = 0;
    for (float f : history)
        sum += f;

    return sum / history.size();
}