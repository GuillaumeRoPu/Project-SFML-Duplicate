#include "Scenes/lascen.h"
#include "Components/Transform.h"
#include "Objects/Bullet.h"

lascen::lascen()
{
	p = CreateGameObject<Player>();
	caca = CreateGameObject<EnemyManager>();
	blocker = CreateGameObject<Blocker>();
}

void lascen::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
	window.clear(sf::Color(0,125,125,255));

	Scene::PlayScene(window, input);

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

	for (Enemy* enemy : caca->_enemyManagerController->_allEnemies) {
		sf::RectangleShape grocaca(sf::Vector2f(20, 20));
		grocaca.setFillColor(sf::Color::Black);
		grocaca.setOrigin({ 10,10 });
		enemy->_rb->mass = 1;
		grocaca.setPosition(sf::Vector2f(enemy->transform->position.x() * pixelPerPoint, enemy->transform->position.y() * pixelPerPoint));

		for (Bullet* bullet : enemy->_controller->_bullets)
		{
			sf::CircleShape aieaieaie(10);
			aieaieaie.setFillColor(sf::Color::Yellow);
			aieaieaie.setOrigin(10, 10);
			bullet->_rb->mass = 1;
			aieaieaie.setPosition(sf::Vector2f(bullet->transform->position.x() * pixelPerPoint, bullet->transform->position.y() * pixelPerPoint));
			window.draw(aieaieaie);
		}

		if (enemy->active)
		{
			window.draw(grocaca);
		}
	}


	float pBlockerAngle = blocker->_phc->angle;
	blocker->transform->position.RotateAroundCenter(p->transform->position, 1.3f, pBlockerAngle);
	blocker->transform->rotation = pBlockerAngle;
	sf::RectangleShape pBlocker({ blocker->transform->scale.x() * pixelPerPoint, blocker->transform->scale.y() * pixelPerPoint });
	pBlocker.setFillColor(sf::Color::Green);
	pBlocker.setOrigin({25, 5});
	pBlocker.setPosition({ blocker->transform->position.x() * pixelPerPoint, blocker->transform->position.y() * pixelPerPoint });
	pBlocker.setRotation(pBlockerAngle + 90);


	window.draw(pshape);
	window.draw(pBlocker);
}
