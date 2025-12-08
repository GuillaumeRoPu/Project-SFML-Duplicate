#include "Scenes/MainMenu.h"
#include "Manager/FontManager.h"
#include "Manager/SceneManager.h"
#include "Scenes/lascen.h"
#include "Engine/Time.h"
#include <SFML/Graphics.hpp>
#include <cmath>

MainMenu::MainMenu()
{
}

void MainMenu::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
    Scene::PlayScene(window, input);

    static float animTime = 0.0f;
    animTime += Time::DeltaTime() * 3;
    float scale = 1.0f + std::sin(animTime) * 0.05f;

    static bool startTransition = false;
    static float flashAlpha = 0.f;
    static float fadeAlpha = 0.f;

    sf::Text title;
    title.setFont(FontManager::f_pixelify);
    title.setString("2D Game");
    title.setCharacterSize(64);
    title.setFillColor(sf::Color::White);
    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(400.f, 200.f);
    title.setScale(scale, scale);

    sf::Text shadow = title;
    shadow.setFillColor(sf::Color(0, 0, 0, 150));
    shadow.setScale(scale * 1.02f, scale * 1.02f);
    shadow.setPosition(400.f + 4, 200.f + 4);

    sf::Text pressKey;
    pressKey.setFont(FontManager::f_pixelify);
    pressKey.setString("ENTREE pour jouer");
    pressKey.setCharacterSize(28);
    pressKey.setFillColor(sf::Color(200, 200, 200));
    pressKey.setPosition(200.f, 320.f);

    window.draw(shadow);
    window.draw(title);

    if (!startTransition)
        window.draw(pressKey);

    if (!startTransition && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        startTransition = true;
        flashAlpha = 200.f;
    }

    if (startTransition && flashAlpha > 0.f)
    {
        flashAlpha -= 600.f * Time::DeltaTime();

        sf::RectangleShape flashRect;
        flashRect.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        flashRect.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)flashAlpha));
        window.draw(flashRect);

        return;
    }

    if (startTransition)
    {
        fadeAlpha += 200.f * Time::DeltaTime();
        if (fadeAlpha >= 255.f)
        {
            fadeAlpha = 255.f;
            SceneManager::LoadScene(1);
        }

        sf::RectangleShape fadeRect;
        fadeRect.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        fadeRect.setFillColor(sf::Color(0, 0, 0, (sf::Uint8)fadeAlpha));
        window.draw(fadeRect);
    }
}
