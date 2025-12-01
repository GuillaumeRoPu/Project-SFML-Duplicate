#include <Scripts/PlayerController.h>
#include "Scripts/BulletController.h"
#include "Objects/Blocker.h"
#include "InputSystem.h"

void PlayerController::Awake()
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_rb->damping = 1;
	_health = owner->GetComponent<HealthComponent>();
}

void PlayerController::Update()
{
	Blocker* blocker = scene().FindFirstObjectByType<Blocker>();
	blocker->GetComponent<PlayerHealthBlocker>()->SetHP(_health->GetHealth().GetRatio());
}

void PlayerController::Move(int verDir, int horDir)
{
	if (verDir == 1) {
		_rb->velocity -= Vector2(0.0f, acceleration * Time::DeltaTime());
	}
	if (verDir == -1) {
		_rb->velocity -= Vector2(0.0f, -acceleration * Time::DeltaTime());
	}
	if (horDir == 1) {
		_rb->velocity -= Vector2(-acceleration * Time::DeltaTime(), 0.0f);
	}
	if (horDir == -1) {
		_rb->velocity -= Vector2(acceleration * Time::DeltaTime(), 0.0f);
	}
}

void PlayerController::TakeDamage(int amount)
{
	_health->TakeDamage(amount);
}

void PlayerController::Heal(int amount)
{
	_health->Heal(amount);
}
