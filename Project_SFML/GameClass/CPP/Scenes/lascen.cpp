#include "Scenes/lascen.h"

lascen::lascen()
{
	caca = CreateGameObject<Enemy>();
}

void lascen::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
	window.clear(sf::Color::Red);

	Scene::PlayScene(window, input);

	sf::RectangleShape grocaca(sf::Vector2f(100, 100));
	grocaca.setFillColor(sf::Color::Black);
	caca->_rb->mass = 1;
	float x = caca->_rb->transform().position.x();
	std::cout << x << std::endl;
	grocaca.setPosition(sf::Vector2f(caca->transform->position.x(), caca->transform->position.y()));
	window.draw(grocaca);
}