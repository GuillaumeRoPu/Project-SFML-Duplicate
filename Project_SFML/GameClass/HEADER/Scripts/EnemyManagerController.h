#pragma once
#include "Components/Scripting.h"
#include "Objects/Enemy.h"
#include <list>

class EnemyManagerController : public Scripting
{
	void Update() override;
	void Start() override;
	void Awake() override;

public:
	void SpawnEnemy();
	void DestroyEnemy(GameObject* enemy);
	Enemy* InstantiateEnemy();
	void ChooseEnemyBullets(Enemy* newEnemy);

public:
	std::list<Enemy*> _allEnemies;
	Player* _player;
	float _cooldown;
	float _timer;
	Vector2 _spawnRadius;
};