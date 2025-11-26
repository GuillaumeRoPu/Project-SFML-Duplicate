#include "Scenes/lascen.h"

lascen::lascen()
{
	caca = CreateGameObject<Enemy>();
}

void lascen::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
	window.clear(sf::Color::Red);
	sf::RectangleShape grocaca(sf::Vector2f(100,100));
	grocaca.setFillColor(sf::Color::Black);
	grocaca.setPosition(sf::Vector2f(caca->transform.position.x(), caca->transform.position.y()));
	window.draw(grocaca);
}
