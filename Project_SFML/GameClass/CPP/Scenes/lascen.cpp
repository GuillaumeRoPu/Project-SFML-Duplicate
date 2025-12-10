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
}

void lascen::PlayScene(sf::RenderWindow& window, InputSystem& input)
{

	Scene::PlayScene(window, input);

	p->_pc->PlayerInputTreatment(input);
	blocker->_phc->BlockerInputTreatment(input);

	//Boucle
	int playerSize = 25;
	p->_pc->BounceOnBorder(window.getSize().x, window.getSize().y, playerSize);


	sf::CircleShape pshape(playerSize);
	pshape.setOrigin(playerSize, playerSize);

	pshape.setFillColor(sf::Color::White);
	sf::Vector2f playerPosition = { p->transform->position.x() * pixelPerPoint, p->transform->position.y() * pixelPerPoint };
	pshape.setPosition(playerPosition);

	for (Enemy* enemy : enemyManager->_enemyManagerController->_allEnemies) {
		enemy->_controller->BounceOnBorder(window.getSize().x, window.getSize().y);
		sf::ConvexShape grocaca(3);
		grocaca.setRotation(Vector2::Angle(Vector2::right(), p->transform->position - enemy->transform->position));
		grocaca.setOutlineColor(sf::Color::White);
		grocaca.setOutlineThickness(1);
		grocaca.setPoint(0, sf::Vector2f(-enemy->transform->scale.x() / 2, -enemy->transform->scale.y() / 2) * pixelPerPoint);
		grocaca.setPoint(1, sf::Vector2f(enemy->transform->scale.y() / 2, 0) * pixelPerPoint);
		grocaca.setPoint(2, sf::Vector2f(-enemy->transform->scale.x() / 2, enemy->transform->scale.y() / 2) * pixelPerPoint);

		grocaca.setFillColor(sf::Color::Black);
		grocaca.setOrigin({ enemy->transform->scale.x()/2, enemy->transform->scale.y()/2 });
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


	int pBlockerAngle = blocker->_phc->angle;
	blocker->transform->position.RotateAroundCenter(p->transform->position, 1.f, pBlockerAngle + 90);
	blocker->transform->rotation = pBlockerAngle;
	PlayerHealthBlocker* healthBlocker = blocker->GetComponent<PlayerHealthBlocker>();

	sf::Vector2f blockerHealthSize = { healthBlocker->GetHealthSize() * pixelPerPoint, blocker->transform->scale.y() * pixelPerPoint };
	sf::Vector2f blockerSize = { blocker->transform->scale.x() * pixelPerPoint, blocker->transform->scale.y() * pixelPerPoint };
	
	sf::RectangleShape pBlocker(blockerHealthSize);
	pBlocker.setFillColor(sf::Color::Green);
	pBlocker.setOrigin({ blockerSize.x/2, blockerSize.y/2});
	pBlocker.setPosition({ blocker->transform->position.x() * pixelPerPoint, blocker->transform->position.y() * pixelPerPoint });
	pBlocker.setRotation(pBlockerAngle);

	sf::RectangleShape pBlockerBackground(blockerSize);
	pBlockerBackground.setFillColor(sf::Color::Red);
	pBlockerBackground.setOrigin({ blockerSize.x/2, blockerSize.y/2 });
	pBlockerBackground.setPosition({ blocker->transform->position.x() * pixelPerPoint, blocker->transform->position.y() * pixelPerPoint });
	pBlockerBackground.setRotation(pBlockerAngle);


	window.draw(pshape);
	pshape.setRadius(playerSize - 1);
	pshape.setOrigin(playerSize - 1, playerSize - 1);
	pshape.setFillColor(sf::Color::Black);
	window.draw(pshape);

	window.draw(pBlockerBackground);
	window.draw(pBlocker);


	// UI
	sf::Text score;
	score.setFont(FontManager::f_pixelify);
	score.setString("score : " + std::to_string(p->GetComponent<PlayerController>()->score));
	score.setPosition(window.mapPixelToCoords({ 650, 0 }));
	window.draw(score);
}
