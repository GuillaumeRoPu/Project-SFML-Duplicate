#include "Objects/Bullet.h"

Bullet::Bullet() 
{
	transform->scale = { 0.4f, 0.4f };

	_rb = AddComponent<Rigidbody2D>();
	_collider = AddComponent<CircleCollider2D>();
	_controller = AddComponent<BulletController>();
	_collider->isTrigger = true;
	name = "Bullet";
}