#include "Objects/Player.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components/Physics/Rigidbody2D.h"

Player::Player()
{

	_rb = AddComponent<Rigidbody2D>();
	_cc = AddComponent<CircleCollider2D>();
	_pc = AddComponent<PlayerController>();

	name = "Player";
	_health = AddComponent<HealthComponent>();
	_health->SetMaxHealth(10, true);
}