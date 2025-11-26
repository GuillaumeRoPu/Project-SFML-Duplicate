#include "Scenes/lascen.h"
#include "Object/gmtest.h"

lascen::lascen()
{
	auto a = CreateGameObject<gmtest>();
}

void lascen::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
	window.clear(sf::Color::Red);
}
