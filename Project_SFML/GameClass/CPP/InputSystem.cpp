#include "InputSystem.h"
#include <iostream>

InputSystem::InputSystem()
{
    button.anyKey = button.left = button.right = button.up = button.down = button.rotateRight = button.rotateLeft = button.enter = button.lshift = false;
}

void InputSystem::CheckEventInput(sf::Event event, sf::RenderWindow& window)
{
    // Regarde quand une touche est préssé
    if (event.type == sf::Event::KeyPressed)
    {
        const sf::Event::KeyEvent& keyPressed = event.key;
        button.anyKey = true;
        switch (keyPressed.code)
        {
            // Ferme la fenêtre
            case sf::Keyboard::Escape:
                window.close();
                break;

            case sf::Keyboard::LShift:
                button.lshift = true;
                break;

            case sf::Keyboard::Enter:
                button.enter = true;
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
        button.anyKey = false;
        const sf::Event::KeyEvent& keyReleased = event.key;
        switch (keyReleased.code)
        {

            case sf::Keyboard::Enter:
                button.enter = false;
                break;

            case sf::Keyboard::LShift:
                button.lshift = false;
                break;

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
            {
                button.anyKey = true;
                break;
            }
        }

    }

    // Regarde quand un bouton de la souris est pressée
    if (event.type == sf::Event::MouseButtonPressed)
    {

    }


    if (sf::Joystick::isConnected(0) && !button.anyKey)
    {

        if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -20)
        {
            button.left = true;
            button.right = false;
        }
        else if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 20)
        {
            button.left = false;
            button.right = true;
        }
        else
        {
            button.left = false;
            button.right = false;
        }
        if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 20)
        {
            button.down = true;
            button.up = false;
        }
        else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -20)
        {
            button.down = false;
            button.up = true;
        }
        else
        {
            button.down = false;
            button.up = false;
        }

        if (sf::Joystick::getAxisPosition(0, sf::Joystick::Z) > 20)
        {
            button.rotateLeft = true;
            button.rotateRight = false;
        }
        else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Z) < -20)
        {
            button.rotateLeft = false;
            button.rotateRight = true;
        }
        else
        {
            button.rotateLeft = false;
            button.rotateRight = false;
        }

        if (sf::Joystick::isButtonPressed(0, 7))
        {
            button.enter = true;
        }
        else
        {
            button.enter = false;
        }

        if (sf::Joystick::isButtonPressed(0, 0))
        {
            button.lshift = true;
        }
        else
        {
            button.lshift = false;
        }
    }
}

InputSystem::Button InputSystem::getButton()
{
    return button;
}

std::list<bool> InputSystem::GetAllButton()
{
    return { button.left, button.right, button.down, button.up, button.enter, button.lshift, button.rotateLeft, button.rotateRight };
}
