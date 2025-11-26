#include "Scripts/EnemyController.h"

void EnemyController::FixedUpdate()
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_rb->AddForce(Vector2(1000, 1000));
}
