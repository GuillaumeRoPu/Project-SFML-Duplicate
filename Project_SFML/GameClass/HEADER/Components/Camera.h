#pragma once
#include "Components\Component.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <Engine\FryEngine.h>

class Camera : public Component
{
public:

	Vector2 viewport { 800, 800 };
	sf::Color backgroundColor = sf::Color::Black;

	void SetCameraView(sf::RenderWindow& window);
};