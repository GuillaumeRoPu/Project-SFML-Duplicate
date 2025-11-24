#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>
#include <list>

class InputSystem
{
	struct Button { bool any, left, right, up, down; };

public:
	InputSystem();

	void CheckEventInput(std::optional<sf::Event> event, sf::RenderWindow& window);

	Button getButton();

private:
	InputSystem::Button button;

	std::list<bool> GetAllButton();

};

#endif // !INPUT_H