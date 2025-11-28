#include "Scenes/lascen.h"


lascen::lascen()
{
	p = CreateGameObject<Player>();
	caca = CreateGameObject<Enemy>();
	blocker = CreateGameObject<Blocker>();
}

void lascen::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
	window.clear(sf::Color::Red);

	Scene::PlayScene(window, input);

	//Trucs que vous devez mettre dans le constructeur mais ça fonctionne pas dans le constructeur :'(
	caca->_controller->GetPlayer(this);

	//Boucle
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
	sf::Vector2f playerPosition = { p->transform->position.x() * pixelPerPoint, p->transform->position.y() * pixelPerPoint };
	pshape.setPosition(playerPosition);

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

	for (Bullet* bullet : caca->_controller->_bullets)
	{
		sf::CircleShape aieaieaie(10);
		aieaieaie.setFillColor(sf::Color::Blue);
		aieaieaie.setOrigin(10,10);
		bullet->_rb->mass = 1;
		aieaieaie.setPosition(sf::Vector2f(bullet->transform->position.x() * pixelPerPoint, bullet->transform->position.y() * pixelPerPoint));
		window.draw(aieaieaie);
	}

	sf::Vector2f pBlockerSize(50, 10);
	Vector2 pBlockerPosition = fry::Vector2::RotateAngle(Vector2{ p->transform->position.x() * pixelPerPoint + sizePlayer, p->transform->position.y() * pixelPerPoint + sizePlayer + 20 }, Vector2{ playerPosition.x, playerPosition.y }, blocker->_phc->angle);
	float pBlockerAngle = blocker->_phc->angle;
	blocker->Transformation(pBlockerSize, pBlockerPosition, pBlockerAngle);
	sf::RectangleShape pBlocker(pBlockerSize);
	pBlocker.setFillColor(sf::Color::Green);
	pBlocker.setOrigin(pBlockerSize);
	pBlocker.setPosition({ pBlockerPosition.x(), pBlockerPosition.y()});
	pBlocker.setRotation(pBlockerAngle);

	sf::RectangleShape grocaca(sf::Vector2f(20, 20));
	grocaca.setFillColor(sf::Color::Black);
	grocaca.setOrigin({ 10,10 });
	caca->_rb->mass = 1;
	grocaca.setPosition(sf::Vector2f(caca->transform->position.x() * pixelPerPoint, caca->transform->position.y() * pixelPerPoint));

	window.draw(grocaca);

	window.draw(pshape);
	window.draw(pBlocker);
}
