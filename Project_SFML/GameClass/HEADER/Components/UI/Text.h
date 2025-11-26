#pragma once
#include "Components/Component.h"

class Text : public Component
{
public:

	void Start() override;

	std::string text;
	sf::Color color;
	sf::Font font;

};