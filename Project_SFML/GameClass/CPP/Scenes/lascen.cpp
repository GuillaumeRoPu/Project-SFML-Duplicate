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

	int sizePlayer = 25;
	sf::CircleShape pshape(sizePlayer);
	pshape.setOrigin(sizePlayer, sizePlayer);
	pshape.setPosition({ p->transform->position.x(), p->transform->position.y() });

	float wsx = window.getSize().x;
	float wsy = window.getSize().y;
	if (p->transform->position.x() - sizePlayer < -wsx/2 || p->transform->position.x() + sizePlayer > wsx/2) {
		p->_rb->velocity = Vector2(-p->_rb->velocity.x(), p->_rb->velocity.y());
	}
	if (p->transform->position.y() - sizePlayer < -wsy/2 || p->transform->position.y() + sizePlayer > wsy/2) {
		p->_rb->velocity = Vector2(p->_rb->velocity.x(), -p->_rb->velocity.y());
	}

	window.draw(pshape);
}
