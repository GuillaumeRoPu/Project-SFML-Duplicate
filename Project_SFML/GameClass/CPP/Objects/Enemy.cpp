#include "Objects/Enemy.h"

Enemy::Enemy() 
{
	_collider = AddComponent<BoxCollider2D>();
	_rb = AddComponent<Rigidbody2D>();
	_controller = AddComponent<EnemyController>();
}