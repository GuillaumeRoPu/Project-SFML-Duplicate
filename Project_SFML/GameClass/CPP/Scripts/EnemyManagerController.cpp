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
	
}

void EnemyManagerController::GetPlayer(Scene* scene) {
	_player = scene->FindFirstObjectByType<Player>();
	_scene = scene;
}

void EnemyManagerController::SpawnEnemy() 
{
	Enemy* newEnemy = InstantiateEnemy();
	if (newEnemy != nullptr && _player != nullptr) {
		newEnemy->GetComponent<EnemyController>()->GetPlayer(_player);
		newEnemy->GetComponent<EnemyController>()->GetScene(_scene);
		Vector2 position = Vector2(Random::Range(-100 / (int)pixelPerPoint, 100 / (int)pixelPerPoint), Random::Range(-100 / (int)pixelPerPoint, 100 / (int)pixelPerPoint));
		newEnemy->transform->position = (position);
	}
	_allEnemies.push_back(newEnemy);
}

Enemy* EnemyManagerController::InstantiateEnemy()
{
	return _scene->CreateGameObject<Enemy>();
}
