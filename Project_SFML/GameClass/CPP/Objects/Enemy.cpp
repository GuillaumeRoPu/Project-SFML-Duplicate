#include "Objects/Enemy.h"

Enemy::Enemy()
{
	transform->scale = { 2,2 };

	_rb = AddComponent<Rigidbody2D>();
	_collider = AddComponent<BoxCollider2D>();
	_controller = AddComponent<EnemyController>();
	name = "Enemy";
}