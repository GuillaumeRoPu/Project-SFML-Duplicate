#include "Scenes/lascen.h"
#include "Components/Transform.h"
#include "Components/Physics/Rigidbody2D.h"
#include "Objects/Bullet.h"
#include "Components/Camera.h"
#include "Manager\AudioManager.h"

lascen::lascen()
{
	p = CreateGameObject<Player>();
	enemyManager = CreateGameObject<EnemyManager>();
	bulletManager = CreateGameObject<BulletManager>();
	blocker = CreateGameObject<Blocker>();

	// Création du game object permettant aux détecter lorsqu'elles sortent de la caméra.
	GameObject* limit = CreateGameObject<GameObject>();
	limit->transform->scale = Vector2(16, 16);
	limit->name = "Limit";
	BoxCollider2D* boxLimit = limit->AddComponent<BoxCollider2D>();
	boxLimit->isTrigger = true;

	Time::timeScale = 0;
}

void lascen::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
	Scene::PlayScene(window, input);

	//Boucle
	if (input.getButton().any && !p->_health->IsDead())
		Time::timeScale = 1;
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
	pshape.setFillColor(sf::Color::White);
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
		{
			AudioManager::PlaySound(AudioManager::collision);
			p->_rb->velocity = Vector2(-p->_rb->velocity.x(), p->_rb->velocity.y());

		}
	}
	if (p->transform->position.x() * pixelPerPoint + sizePlayer > wsx / 2) {
		if (p->_rb->velocity.x() >= 0)
		{
			p->_rb->velocity = Vector2(-p->_rb->velocity.x(), p->_rb->velocity.y());
			AudioManager::PlaySound(AudioManager::collision);
		}
	}
	if (p->transform->position.y() * pixelPerPoint - sizePlayer < -wsy / 2) {
		if (p->_rb->velocity.y() <= 0)
		{
			AudioManager::PlaySound(AudioManager::collision);
			p->_rb->velocity = Vector2(p->_rb->velocity.x(), -p->_rb->velocity.y());
		}
	}
	if (p->transform->position.y() * pixelPerPoint + sizePlayer > wsy / 2) {
		if (p->_rb->velocity.y() >= 0)
		{
			AudioManager::PlaySound(AudioManager::collision);
			p->_rb->velocity = Vector2(p->_rb->velocity.x(), -p->_rb->velocity.y());
		}
	}

	for (Enemy* enemy : enemyManager->_enemyManagerController->_allEnemies) {
		sf::RectangleShape grocaca(sf::Vector2f(20, 20));
		grocaca.setFillColor(sf::Color::White);
		grocaca.setOrigin({ 10,10 });
		grocaca.setPosition(sf::Vector2f(enemy->transform->position.x() * pixelPerPoint, enemy->transform->position.y() * pixelPerPoint));

		if (enemy->active)
		{
			window.draw(grocaca);
		}

		grocaca.setSize(sf::Vector2f(18, 18));
		grocaca.setFillColor(sf::Color::Black);
		grocaca.setOrigin({ 10-1,10-1 });
		grocaca.setPosition(sf::Vector2f(enemy->transform->position.x() * pixelPerPoint, enemy->transform->position.y() * pixelPerPoint));

		if (enemy->active)
		{
			window.draw(grocaca);
		}
	}

	for (Bullet* bullet : bulletManager->_bulletManagerController->_allBullets) {
		sf::CircleShape grocaca(10);
		grocaca.setFillColor(sf::Color::Yellow);
		grocaca.setOrigin({ 10,10 });
		grocaca.setPosition(sf::Vector2f(bullet->transform->position.x() * pixelPerPoint, bullet->transform->position.y() * pixelPerPoint));

		if (bullet->active)
		{
			window.draw(grocaca);
		}

		grocaca.setRadius(10-1);
		grocaca.setFillColor(sf::Color::Black);
		grocaca.setOrigin({ 10 - 1,10 - 1 });
		grocaca.setPosition(sf::Vector2f(bullet->transform->position.x() * pixelPerPoint, bullet->transform->position.y() * pixelPerPoint));

		if (bullet->active)
		{
			window.draw(grocaca);
		}
	}


	float pBlockerAngle = blocker->_phc->angle;
	blocker->transform->position.RotateAroundCenter(p->transform->position, 1.3f, pBlockerAngle);
	blocker->transform->rotation = pBlockerAngle;
	PlayerHealthBlocker* healthBlocker = blocker->GetComponent<PlayerHealthBlocker>();

	sf::Vector2f blockerHealthSize = { healthBlocker->GetHealthSize() * pixelPerPoint, blocker->transform->scale.y() * pixelPerPoint };
	sf::Vector2f blockerSize = { blocker->transform->scale.x() * pixelPerPoint, blocker->transform->scale.y() * pixelPerPoint };
	
	sf::RectangleShape pBlocker(blockerHealthSize);
	pBlocker.setFillColor(sf::Color::Green);
	pBlocker.setOrigin({ blockerSize.x/2, blockerSize.y/2});
	pBlocker.setPosition({ blocker->transform->position.x() * pixelPerPoint, blocker->transform->position.y() * pixelPerPoint });
	pBlocker.setRotation(pBlockerAngle + 90);

	sf::RectangleShape pBlockerBackground(blockerSize);
	pBlockerBackground.setFillColor(sf::Color::Red);
	pBlockerBackground.setOrigin({ blockerSize.x/2, blockerSize.y/2 });
	pBlockerBackground.setPosition({ blocker->transform->position.x() * pixelPerPoint, blocker->transform->position.y() * pixelPerPoint });
	pBlockerBackground.setRotation(pBlockerAngle + 90);


	window.draw(pshape);
	pshape.setRadius(sizePlayer-1);
	pshape.setOrigin(sizePlayer-1, sizePlayer-1);
	pshape.setFillColor(sf::Color::Black);
	window.draw(pshape);

	window.draw(pBlockerBackground);
	window.draw(pBlocker);

	if (fadeAlpha > 0.f)
	{
		fadeAlpha -= 200.f * Time::DeltaTime();
		if (fadeAlpha < 0.f) fadeAlpha = 0.f;

		sf::RectangleShape fadeRect;
		fadeRect.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
		fadeRect.setFillColor(sf::Color(0, 0, 0, (sf::Uint8)fadeAlpha));
		window.draw(fadeRect);
	}
}
