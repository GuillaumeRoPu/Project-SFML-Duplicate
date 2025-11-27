#pragma once
#include "Components/Component.h"
#include <iostream>

class SpriteRenderer : public Component
{
public:
	SpriteRenderer();
	SpriteRenderer(std::string path);

	sf::Color color = sf::Color::White;
	//sf::Sprite sprite = sf::Sprite();
	void SetTexture(std::string path);
	sf::Texture GetTexture() { return texture; }

private:
	sf::Texture texture;
	
};