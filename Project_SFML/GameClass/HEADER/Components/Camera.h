#pragma once
#include "Components\Component.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <Engine\FryEngine.h>

class Camera : public Component
{
public:

	Vector2 viewport { 800, 800 };

	void SetCameraView(sf::RenderWindow& window);
};