#include "Scenes/lascen.h"
#include "Components/Transform.h"
#include "Components/Physics/Rigidbody2D.h"
#include "Objects/Bullet.h"
#include "Scripts\MusicController.h"
#include "Components/Camera.h"
#include "Manager\AudioManager.h"
#include "Manager\SceneManager.h"

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
	limit->layer = LayerManager::GetLayerByName("Limit");
	BoxCollider2D* boxLimit = limit->AddComponent<BoxCollider2D>();
	boxLimit->isTrigger = true;
	boxLimit->includeLayers += LayerManager::GetLayerByName("Limit").value;

	GameObject* sceneController = CreateGameObject<GameObject>();
	MusicController* music = sceneController->AddComponent<MusicController>();
	music->gameMusic = true;
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
		grocaca.setOutlineColor(enemy->_controller->color);
		grocaca.setOutlineThickness(1);
		grocaca.setPoint(0, sf::Vector2f(-enemy->transform->scale.x() / 2, -enemy->transform->scale.y() / 2) * pixelPerPoint);
		grocaca.setPoint(1, sf::Vector2f(enemy->transform->scale.y() / 2, 0) * pixelPerPoint);
		grocaca.setPoint(2, sf::Vector2f(-enemy->transform->scale.x() / 2, enemy->transform->scale.y() / 2) * pixelPerPoint);

		grocaca.setFillColor(sf::Color::Black);
		grocaca.setOrigin({enemy->transform->scale.x()/2, enemy->transform->scale.y()/2 });
		grocaca.setPosition(sf::Vector2f(enemy->transform->position.x() * pixelPerPoint, enemy->transform->position.y() * pixelPerPoint));

		if (enemy->active)
		{
			window.draw(grocaca);
		}
	}

	for (Bullet* bullet : bulletManager->_bulletManagerController->_allBullets) {
		sf::CircleShape grocaca(bullet->transform->scale.x() * pixelPerPoint / 2);
		grocaca.setFillColor(bullet->_controller->_color);
		grocaca.setOrigin({ bullet->transform->scale.x() * pixelPerPoint / 2, bullet->transform->scale.x() * pixelPerPoint / 2 });
		grocaca.setPosition(sf::Vector2f(bullet->transform->position.x() * pixelPerPoint, bullet->transform->position.y() * pixelPerPoint));

		if (bullet->active)
		{
			window.draw(grocaca);
		}

		grocaca.setRadius((bullet->transform->scale.x()) * pixelPerPoint / 2 - 1);
		grocaca.setFillColor(sf::Color::Black);
		grocaca.setOrigin({ (bullet->transform->scale.x()) * pixelPerPoint / 2 - 1, (bullet->transform->scale.x()) * pixelPerPoint / 2 - 1});
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
	pshape.setRadius(playerSize / 3);
	pshape.setOrigin(playerSize / 3, playerSize / 3);
	pshape.setFillColor(sf::Color(255, 255, 255, p->_pc->DashtoDashTimerFloat() * 0.5f * 255));
	if (p->_pc->DashtoDashTimerFloat() == 1) 
	{
		pshape.setFillColor(sf::Color(255, 255, 255, 255));
	}
	window.draw(pshape);

	window.draw(pBlockerBackground);
	window.draw(pBlocker);

	// Time before heal
	if (_healTimer <= 0)
	{
		p->GetComponent<HealthComponent>()->Heal(20);
		_healTimer = _healDuration;
	}
	else
	{
		_healTimer -= Time::DeltaTime();
	}

	// Time before end of the game
	if (p->GetComponent<HealthComponent>()->IsDead())
	{
		_gameoverTime += Time::UnscaledDeltaTime();
		if (_gameoverTime >= _gameoverDuration)
		{
			window.close();
		}
	}


	// UI
	sf::Text score;
	score.setFont(FontManager::f_pixelify);
	score.setString("score : " + std::to_string(p->GetComponent<PlayerController>()->score));
	score.setPosition(window.mapPixelToCoords({ 650, 0 }));
	window.draw(score);

	sf::Text healTimer;
	healTimer.setFont(FontManager::f_pixelify);
	healTimer.setString("Temps restant avant le prochain soin : " + std::to_string((int)_healTimer));
	healTimer.setCharacterSize(23);
	healTimer.setOrigin(healTimer.getLocalBounds().width / 2, healTimer.getLocalBounds().height / 2);
	healTimer.setPosition(window.mapPixelToCoords({ 400, 700 }));
	if (_healTimer <= 10)
		window.draw(healTimer);


	//FADE
	if (fadeAlpha > 0.f)
	{
		fadeAlpha -= 200.f * Time::DeltaTime();
		if (fadeAlpha < 0.f) fadeAlpha = 0.f;

		sf::Vector2u winSize = window.getSize();

		sf::Vector2f worldTopLeft = window.mapPixelToCoords(sf::Vector2i(0, 0));
		sf::Vector2f worldBottomRight = window.mapPixelToCoords(
			sf::Vector2i(static_cast<int>(winSize.x), static_cast<int>(winSize.y))
		);

		sf::Vector2f worldSize(worldBottomRight.x - worldTopLeft.x,
			worldBottomRight.y - worldTopLeft.y);

		sf::RectangleShape fadeRect;
		fadeRect.setSize(worldSize);
		fadeRect.setPosition(worldTopLeft);
		fadeRect.setFillColor(sf::Color(0, 0, 0, (sf::Uint8)fadeAlpha));
		window.draw(fadeRect);
	}
}
