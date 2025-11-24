#include "LaunchMenu.h"
#include "Manager/SceneManager.h"

void LaunchMenu::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
    //PlayScene(window, input);

    float _time = (float)clock() / (float)CLOCKS_PER_SEC;

    // Création du text d'appui
    sf::Text pressAnyButtonTxt(FontManager::f_pixelify);
    pressAnyButtonTxt.setCharacterSize(26);
    pressAnyButtonTxt.setFillColor(sf::Color::White);
    pressAnyButtonTxt.setPosition(sf::Vector2f(400, 600));
    pressAnyButtonTxt.setString(_pressAnyButtonString);

    // Création du titre
    sf::Text titleTxt(FontManager::f_pixelify);
    titleTxt.setCharacterSize(100);
    titleTxt.setFillColor(sf::Color::Red);
    titleTxt.setString("2D GAME");
    titleTxt.setPosition(sf::Vector2f(400 - titleTxt.getGlobalBounds().size.x / 2, 100));

    // Effet de texte
    bool txtGenerate = _pressAnyButtonString == "Appui sur une touche pour jouer !";
    if (!txtGenerate)
    {
        pressAnyButtonTxt.setString(TextEffect::TypeWriter("Appui sur une touche pour jouer !", pressAnyButtonTxt, 0.07f));
        pressAnyButtonTxt.setPosition(sf::Vector2f(400 - pressAnyButtonTxt.getGlobalBounds().size.x / 2, 600));
        _pressAnyButtonString = pressAnyButtonTxt.getString();
    }
    pressAnyButtonTxt.setPosition(sf::Vector2f(400 - pressAnyButtonTxt.getGlobalBounds().size.x / 2, 600));

    // Clignote le texte
    //if ((int)_time % 2 == 0 || !txtGenerate)
    window.draw(pressAnyButtonTxt);
    window.draw(titleTxt);

    if (input.getButton().any)
    {
        SceneManager::LoadScene(1);
    }
}
