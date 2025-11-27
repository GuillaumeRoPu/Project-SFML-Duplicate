#include "Scenes/lascen.h"


lascen::lascen()
{
	p = CreateGameObject<Player>();
}

void lascen::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
	window.clear(sf::Color::Red);

	Scene::PlayScene(window, input);

	if (input.getButton().up) {
		p->_pc->Move(1, 0);
	}
	else if (input.getButton().down) {
		p->_pc->Move(-1, 0);
	}
	if (input.getButton().right) {
		p->_pc->Move(0, 1);
	}
	else if (input.getButton().left) {
		p->_pc->Move(0, -1);
	}
	
	std::cout << p->_rb->velocity.x() << " " << p->_rb->velocity.y() << std::endl;

	sf::CircleShape pshape(25);
	pshape.setOrigin(25, 25);
	pshape.setPosition({ p->transform.position.x(), p->transform.position.y() });
	window.draw(pshape);

	
}
