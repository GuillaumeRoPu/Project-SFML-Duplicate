#include "Scenes/MainMenu.h"
#include "Manager/FontManager.h"
#include "Manager/SceneManager.h"
#include "Scenes/lascen.h"
#include "Engine/Time.h"
#include <SFML/Graphics.hpp>
#include <cmath>

MainMenu::MainMenu()
{
    MainCamera->transform->position = { 8,8 };
}

void MainMenu::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
    Scene::PlayScene(window, input);
    static float animTime = 0.0f;
    animTime += Time::DeltaTime() * 3;
    float scale = 1.0f + std::sin(animTime) * 0.05f;

    sf::Text title;
    title.setFont(FontManager::f_pixelify);
    title.setString("2D Game");
    title.setCharacterSize(64);
    title.setFillColor(sf::Color::White);
    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(400.f, 200.f);
    title.setScale(scale, scale);

    sf::Text shadow = title;
    shadow.setFillColor(sf::Color(0, 0, 200, 150));
    shadow.setScale(scale * 1.02f, scale * 1.02f);
    shadow.setPosition(400.f + 4, 200.f + 4);

    sf::Text pressKey;
    pressKey.setFont(FontManager::f_pixelify);
    pressKey.setString("ENTREE pour jouer");
    pressKey.setCharacterSize(28);
    pressKey.setFillColor(sf::Color(200, 200, 200));
    pressKey.setOrigin({ pressKey.getGlobalBounds().width/2, pressKey.getGlobalBounds().height / 2 });
    pressKey.setPosition(400.f, 620.f);

    window.draw(shadow);
    window.draw(title);

    window.draw(pressKey);

    if (input.getButton().enter)
        SceneManager::LoadScene(1);
}
