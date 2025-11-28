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
	_cooldown = 5;
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
		Vector2 position = Vector2(Random::Range(-100 / (int)pixelPerPoint, 100 / (int)pixelPerPoint), Random::Range(-100 / (int)pixelPerPoint, 100 / (int)pixelPerPoint));
		newEnemy->transform->position = (position);
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
