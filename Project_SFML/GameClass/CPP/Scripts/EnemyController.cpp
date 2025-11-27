#include "Scripts/EnemyController.h"

void EnemyController::FixedUpdate()
{
	//_rb->AddForce(Vector2(1, 1));
	//if (_rb->transform().position.x() > 1000 || _rb->transform().position.x() < -1000)
	//{
		_rb->transform().position = Vector2(0, 0);
	//}
}

void EnemyController::Start() 
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_collider = owner->GetComponent<BoxCollider2D>();
	_collider->bounds.setSize(Vector2(100, 100));
}