#pragma once
#include "Scene.h"
#include "Components/Scripting.h"
#include "Objects/Bullet.h"
#include <list>

class Bullet;
class Player;

class BulletManagerController : public Scripting
{
	void Start() override;
	void Awake() override;

public:
	void SpawnBullet(Vector2 position);
	void DestroyBullet(GameObject* bullet);
	Bullet* InstantiateBullet();

public:
	std::list<Bullet*> _allBullets;
	Player* _player;
	float _cooldown;
	float _timer;
	Vector2 _spawnRadius;
};
