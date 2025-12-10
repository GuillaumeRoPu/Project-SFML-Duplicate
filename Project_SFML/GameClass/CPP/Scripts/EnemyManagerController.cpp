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
	_cooldown = 1;
	_timer = 0;
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
		newEnemy->_controller->SetupBullet(0.5f, 10, 0.5f, 0, sf::Color::Yellow, false, false, 3);
	}
	else if (random == 1) 
	{
		newEnemy->_controller->SetupBullet(2, 5, 0.2f, 0, sf::Color::Cyan, true, false, 3);
	}
	else if (random == 2)
	{
		newEnemy->_controller->SetupBullet(5, 0, 1, 0, sf::Color::Magenta, false, true, 3);
	}
	else if (random == 3)
	{
		newEnemy->_controller->SetupBullet(15, 0, 0.5f, 0, sf::Color::Green, false, true, 3);
	}
	else if (random == 4)
	{
		newEnemy->_controller->SetupBullet(1, 40, 2, 0, sf::Color::Blue, false, false, 3);
	}
}
