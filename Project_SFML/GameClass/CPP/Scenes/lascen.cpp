#include "Scenes/lascen.h"


lascen::lascen()
{
	p = CreateGameObject<Player>();
	caca = CreateGameObject<Enemy>();
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
	pshape.setPosition({ p->transform->position.x() * pixelPerPoint, p->transform->position.y() * pixelPerPoint });

	float wsx = window.getSize().x;
	float wsy = window.getSize().y;
	if (p->transform->position.x()* pixelPerPoint - sizePlayer < -wsx / 2 || p->transform->position.x()* pixelPerPoint + sizePlayer > wsx / 2) {
		p->_rb->velocity = Vector2(-p->_rb->velocity.x(), p->_rb->velocity.y());
	}
	if (p->transform->position.y()* pixelPerPoint - sizePlayer < -wsy / 2 || p->transform->position.y()* pixelPerPoint + sizePlayer > wsy / 2) {
		p->_rb->velocity = Vector2(p->_rb->velocity.x(), -p->_rb->velocity.y());
	}

	sf::RectangleShape grocaca(sf::Vector2f(100, 100));
	grocaca.setFillColor(sf::Color::Black);
	grocaca.setOrigin({ 50,50 });
	caca->_rb->mass = 1;
	grocaca.setPosition(sf::Vector2f(caca->transform->position.x() * pixelPerPoint, caca->transform->position.y() * pixelPerPoint));

	window.draw(grocaca);

	window.draw(pshape);
}
