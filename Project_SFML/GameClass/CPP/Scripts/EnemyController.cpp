#include "Scripts/EnemyController.h"

void EnemyController::FixedUpdate()
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_rb->AddForce(Vector2(10,10));
	std::cout << _rb->velocity.x() << std::endl;
}
