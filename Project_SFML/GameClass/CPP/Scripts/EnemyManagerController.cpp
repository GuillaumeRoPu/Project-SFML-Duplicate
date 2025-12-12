#include "Scripts/EnemyManagerController.h"

void EnemyManagerController::Update()
{
	_timer += Time::DeltaTime();
	if (_cooldown <= _timer) 
	{
		_timer = 0;
		SpawnEnemy();
	}
	if (Time::timeScale < 1.0f && Time::timeScale != 0) 
	{
		Time::timeScale = Mathf::Lerp(Time::timeScale, 1, (1-Time::timeScale)*0.1f);
		if (Time::timeScale > 0.99f) 
		{
			Time::timeScale = 1;
		}
	}
}

void EnemyManagerController::Start()
{
	_cooldown = 3.f;
	_timer = 0.f;
}

void EnemyManagerController::Awake()
{
	_player = scene().FindFirstObjectByType<Player>();
}

void EnemyManagerController::SpawnEnemy() 
{
	Enemy* newEnemy = InstantiateEnemy();
	newEnemy->_controller->controller = this;
	if (newEnemy != nullptr && _player != nullptr) {
		Vector2 position = Vector2(Random::Range((float)-5, (float)5), Random::Range((float)-5, (float)5));
		newEnemy->transform->position = (position);
		ChooseEnemyBullets(newEnemy);
	}
	_allEnemies.push_back(newEnemy);
}

void EnemyManagerController::DestroyEnemy(GameObject* enemy)
{
	auto it = std::find_if(_allEnemies.begin(), _allEnemies.end(),
		[&](auto& uptr) {return uptr == enemy; });

	if (it != _allEnemies.end())
		_allEnemies.erase(it);

	scene().DestroyObject(enemy);
}


Enemy* EnemyManagerController::InstantiateEnemy()
{
	return scene().CreateGameObject<Enemy>();
}

void EnemyManagerController::ChooseEnemyBullets(Enemy* newEnemy)
{
	int random = Random::Range(0, 100);
	if (random <= 30)
	{
		newEnemy->_controller->SetupBulletProperties(0.5f, 10, 0.5f, 2, sf::Color::Yellow, false, false, 3);
		newEnemy->_controller->SetupEnemyProperties(50, 0.5f, 0.8f, sf::Color::White, EnemyType::Classic);
	}
	else if (random > 30 && random <= 50) 
	{
		newEnemy->_controller->SetupBulletProperties(2, 5, 0.2f, 1, sf::Color::Cyan, true, false, 3);
		newEnemy->_controller->SetupEnemyProperties(40, 0.3f, 0.1f, sf::Color::White, EnemyType::Classic);
	}
	else if (random > 50 && random <= 60)
	{
		newEnemy->_controller->SetupBulletProperties(5, 0, 1, 3, sf::Color::Magenta, false, true, 5);
		newEnemy->_controller->SetupEnemyProperties(70, 1, 2, sf::Color::White, EnemyType::Burst);
		newEnemy->_controller->SetupBurstProperties(0.2f, 5);
	}
	else if (random > 60 && random <= 70)
	{
		newEnemy->_controller->SetupBulletProperties(10, 0, 0.5f, 10, sf::Color::Green, true, false, 3);
		newEnemy->_controller->SetupEnemyProperties(20, 0.2f, 1.5f, sf::Color::White, EnemyType::Classic);
	}
	else if (random > 70 && random <= 80)
	{
		newEnemy->_controller->SetupBulletProperties(1, 40, 2, 5, sf::Color(255,125,0), false, false, 3);
		newEnemy->_controller->SetupEnemyProperties(100, 2, 2, sf::Color::White, EnemyType::Classic);
	}
	else if (random > 80 && random <= 85)
	{
		newEnemy->_controller->SetupBulletProperties(5, 0, 0.5f, 3, sf::Color(255, 125, 255), false, false, 3);
		newEnemy->_controller->SetupEnemyProperties(40, 0.5f, 2, sf::Color::White, EnemyType::Shotgun);
		newEnemy->_controller->SetupShotgunProperties(10, 5);
	}
	else if (random > 85 && random <= 90)
	{
		newEnemy->_controller->SetupBulletProperties(10, -1, 0.2f, 1, sf::Color(125, 125, 255), true, false, 3);
		newEnemy->_controller->SetupEnemyProperties(50, 0.5f, 0.5f, sf::Color::White, EnemyType::Burst);
		newEnemy->_controller->SetupBurstProperties(0.03f, 20);
	}
	else if (random > 90 && random <= 99)
	{
		newEnemy->_controller->SetupBulletProperties(0.5f, -1, 0.3f, 1, sf::Color(255, 125, 125), false, false, 3);
		newEnemy->_controller->SetupEnemyProperties(20, 0.5f, 1, sf::Color::White, EnemyType::Shotgun);
		newEnemy->_controller->SetupShotgunProperties(50, 20);
	}
	else if (random == 100)
	{
		newEnemy->_controller->SetupBulletProperties(0.5f, -1, 1, 1, sf::Color(255, 125, 125), false, false, 3);
		newEnemy->_controller->SetupEnemyProperties(200, 0.5f, 1, sf::Color::White, EnemyType::Shotgun);
		newEnemy->_controller->SetupShotgunProperties(200, 100);
	}
}
