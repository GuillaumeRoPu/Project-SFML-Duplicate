#pragma once

#include "Scene.h"

class MainMenu : public Scene
{
public:
    MainMenu();
    void PlayScene(sf::RenderWindow& window, InputSystem& input) override;
};