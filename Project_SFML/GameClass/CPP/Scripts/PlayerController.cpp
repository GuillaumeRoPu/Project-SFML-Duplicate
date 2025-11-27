#include <Scripts/PlayerController.h>
#include "InputSystem.h"

void PlayerController::Start()
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_rb->damping = 1;
}

void PlayerController::Move(int verDir, int horDir)
{
	if (verDir == 1) {
		_rb->velocity -= Vector2(0.0f, acceleration * Time::DeltaTime());
	}
	else if (verDir == -1) {
		_rb->velocity -= Vector2(0.0f, -acceleration * Time::DeltaTime());
	}
	if (horDir == 1) {
		_rb->velocity -= Vector2(-acceleration * Time::DeltaTime(), 0.0f);
	}
	else if (horDir == -1) {
		_rb->velocity -= Vector2(acceleration * Time::DeltaTime(), 0.0f);
	}
}
