#include "Scripts/EnemyManagerController.h"

void EnemyManagerController::Update()
{
	_timer += Time::DeltaTime();
	if (_cooldown <= _timer) 
	{
		_timer = 0;
		SpawnEnemy();
	}
}

void EnemyManagerController::Start()
{
	_cooldown = 5.f;
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
	int random = Random::Range(0, 4);
	if (random == 0) 
	{
		newEnemy->_controller->SetupBulletProperties(0.5f, 10, 0.5f, 2, sf::Color::Yellow, false, false, 3);
		newEnemy->_controller->SetupEnemyProperties(50, 0.5f, 0.8f, sf::Color::White);
	}
	else if (random == 1) 
	{
		newEnemy->_controller->SetupBulletProperties(2, 5, 0.2f, 1, sf::Color::Cyan, true, false, 3);
		newEnemy->_controller->SetupEnemyProperties(40, 0.3f, 0.1f, sf::Color::White);
	}
	else if (random == 2)
	{
		newEnemy->_controller->SetupBulletProperties(5, 0, 1, 3, sf::Color::Magenta, false, true, 3);
		newEnemy->_controller->SetupEnemyProperties(70, 1, 1, sf::Color::White);
	}
	else if (random == 3)
	{
		newEnemy->_controller->SetupBulletProperties(10, 0, 0.5f, 1, sf::Color::Green, true, false, 3);
		newEnemy->_controller->SetupEnemyProperties(20, 0.2f, 1.5f, sf::Color::White);
	}
	else if (random == 4)
	{
		newEnemy->_controller->SetupBulletProperties(1, 40, 2, 4, sf::Color::Blue, false, false, 3);
		newEnemy->_controller->SetupEnemyProperties(100, 2, 2, sf::Color::White);
	}
}
