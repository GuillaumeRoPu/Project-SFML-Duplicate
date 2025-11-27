#include "Objects/Enemy.h"

Enemy::Enemy()
{
	_rb = AddComponent<Rigidbody2D>();
	_collider = AddComponent<BoxCollider2D>();
	_controller = AddComponent<EnemyController>();
}