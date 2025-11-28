#include "InputSystem.h"
#include <iostream>

InputSystem::InputSystem()
{
    button.any = button.left = button.right = button.up = button.down = button.rotateRight = button.rotateLeft = false;
}

void InputSystem::CheckEventInput(sf::Event event, sf::RenderWindow& window)
{
    // Regarde quand une touche est préssé
    if (event.type == sf::Event::KeyPressed)
    {
        const sf::Event::KeyEvent& keyPressed = event.key;
        button.any = true;
        switch (keyPressed.code)
        {
            // Ferme la fenêtre
            case sf::Keyboard::Escape:
                window.close();
                break;

            case sf::Keyboard::Right:
                button.right = true;
                break;

            case sf::Keyboard::Left:
                button.left = true;
                break;

            case sf::Keyboard::Up:
                button.up = true;
                break;

            case sf::Keyboard::Down:
                button.down = true;
                break;

            case sf::Keyboard::Q:
                button.rotateLeft = true;
                break;

            case sf::Keyboard::D:
                button.rotateRight = true;
                break;
        }
    }

    // Regarde quand une touche est laché
    if (event.type == sf::Event::KeyReleased)
    {
        button.any = false;
        const sf::Event::KeyEvent& keyReleased = event.key;
        switch (keyReleased.code)
        {
            case sf::Keyboard::Right:
                button.right = false;
                break;

            case sf::Keyboard::Left:
                button.left = false;
                break;

            case sf::Keyboard::Up:
                button.up = false;
                break;

            case sf::Keyboard::Down:
                button.down = false;
                break;

            case sf::Keyboard::Q:
                button.rotateLeft = false;
                break;

            case sf::Keyboard::D:
                button.rotateRight = false;
                break;
        }

        for (bool b : GetAllButton())
        {
            if (b)
                button.any = true;
        }

    }

    // Regarde quand un bouton de la souris est pressée
    if (event.type == sf::Event::MouseButtonPressed)
    {

    }
}

InputSystem::Button InputSystem::getButton()
{
    return button;
}

std::list<bool> InputSystem::GetAllButton()
{
    return { button.left, button.right, button.down, button.up };
}
