#include "Scenes/lascen.h"


lascen::lascen()
{
	p = CreateGameObject<Player>();
	caca = CreateGameObject<Enemy>();
	caca->_controller->GetPlayer(this);
	blocker = CreateGameObject<Blocker>();
}

void lascen::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
	window.clear(sf::Color::Red);

	Scene::PlayScene(window, input);

	if (input.getButton().up) {
		p->_pc->Move(1, 0);
	}
	if (input.getButton().down) {
		p->_pc->Move(-1, 0);
	}
	if (input.getButton().right) {
		p->_pc->Move(0, 1);
	}
	if (input.getButton().left) {
		p->_pc->Move(0, -1);
	}
	
	if (input.getButton().rotateRight) {
		blocker->_phc->Rotate(true);
	}
	if (input.getButton().rotateLeft) {
		blocker->_phc->Rotate(false);
	}

	int sizePlayer = 25;
	sf::CircleShape pshape(sizePlayer);
	pshape.setOrigin(sizePlayer, sizePlayer);
	pshape.setPosition({ p->transform->position.x() * pixelPerPoint, p->transform->position.y() * pixelPerPoint });

	float wsx = window.getSize().x;
	float wsy = window.getSize().y;
	/*if (p->transform->position.x()* pixelPerPoint - sizePlayer < -wsx / 2 || p->transform->position.x()* pixelPerPoint + sizePlayer > wsx / 2) {
		p->_rb->velocity = Vector2(-p->_rb->velocity.x(), p->_rb->velocity.y());
	}
	if (p->transform->position.y()* pixelPerPoint - sizePlayer < -wsy / 2 || p->transform->position.y()* pixelPerPoint + sizePlayer > wsy / 2) {
		p->_rb->velocity = Vector2(p->_rb->velocity.x(), -p->_rb->velocity.y());
	}*/

	if (p->transform->position.x() * pixelPerPoint - sizePlayer < -wsx / 2) {
		if (p->_rb->velocity.x() <= 0)
			p->_rb->velocity = Vector2(-p->_rb->velocity.x(), p->_rb->velocity.y());
	}
	if (p->transform->position.x() * pixelPerPoint + sizePlayer > wsx / 2) {
		if (p->_rb->velocity.x() >= 0)
			p->_rb->velocity = Vector2(-p->_rb->velocity.x(), p->_rb->velocity.y());
	}
	if (p->transform->position.y() * pixelPerPoint - sizePlayer < -wsy / 2) {
		if (p->_rb->velocity.y() <= 0)
			p->_rb->velocity = Vector2(p->_rb->velocity.x(), -p->_rb->velocity.y());
	}
	if (p->transform->position.y() * pixelPerPoint + sizePlayer > wsy / 2) {
		if (p->_rb->velocity.y() >= 0)
			p->_rb->velocity = Vector2(p->_rb->velocity.x(), -p->_rb->velocity.y());
	}

	sf::RectangleShape pBlocker(sf::Vector2f(50, 10));
	pBlocker.setFillColor(sf::Color::Green);
	pBlocker.setOrigin({ 50,10 });
	float x = 10 * Mathf::cos(blocker->_phc->rad);
	float y = 10 * Mathf::sin(blocker->_phc->rad);
	std::cout << x << " " << y << std::endl;
	pBlocker.setPosition({ p->transform->position.x() * pixelPerPoint + sizePlayer, p->transform->position.y() * pixelPerPoint + sizePlayer });

	sf::RectangleShape grocaca(sf::Vector2f(20, 20));
	grocaca.setFillColor(sf::Color::Black);
	grocaca.setOrigin({ 10,10 });
	caca->_rb->mass = 1;
	grocaca.setPosition(sf::Vector2f(caca->transform->position.x() * pixelPerPoint, caca->transform->position.y() * pixelPerPoint));

	window.draw(grocaca);

	window.draw(pshape);
	window.draw(pBlocker);
}
