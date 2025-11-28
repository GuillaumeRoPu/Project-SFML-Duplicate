#include "Objects/Enemy.h"

Enemy::Enemy()
{
	transform->scale = { 0.4f, 0.4f };

	_rb = AddComponent<Rigidbody2D>();
	_collider = AddComponent<BoxCollider2D>();
	_collider->isTrigger = true;
	_controller = AddComponent<EnemyController>();
	name = "Enemy";
}