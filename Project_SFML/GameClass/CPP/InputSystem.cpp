#include "InputSystem.h"
#include <iostream>

InputSystem::InputSystem()
{
    button.any = button.left = button.right = button.up = button.down = false;
}

void InputSystem::CheckEventInput(std::optional<sf::Event> event, sf::RenderWindow& window)
{
    // Regarde quand une touche est préssé
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        button.any = true;
        switch (keyPressed->scancode)
        {
            // Ferme la fenêtre
            case sf::Keyboard::Scancode::Escape:
                window.close();
                break;

            case sf::Keyboard::Scancode::Right:
                button.right = true;
                break;

            case sf::Keyboard::Scancode::Left:
                button.left = true;
                break;

            case sf::Keyboard::Scancode::Up:
                button.up = true;
                break;

            case sf::Keyboard::Scancode::Down:
                button.down = true;
                break;
        }
    }

    // Regarde quand une touche est laché
    if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
    {
        button.any = false;
       
        switch (keyReleased->scancode)
        {
            case sf::Keyboard::Scancode::Right:
                button.right = false;
                break;

            case sf::Keyboard::Scancode::Left:
                button.left = false;
                break;

            case sf::Keyboard::Scancode::Up:
                button.up = false;
                break;

            case sf::Keyboard::Scancode::Down:
                button.down = false;
                break;
        }

        for (bool b : GetAllButton())
        {
            if (b)
                button.any = true;
        }

    }

    // Regarde quand un bouton de la souris est pressée
    if (const auto* buttonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        std::cout << buttonPressed->position.x << std::endl;
}

InputSystem::Button InputSystem::getButton()
{
    return button;
}

std::list<bool> InputSystem::GetAllButton()
{
    return { button.left, button.right, button.down, button.up };
}
