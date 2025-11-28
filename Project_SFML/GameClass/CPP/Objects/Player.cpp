#include "Objects/Player.h"

Player::Player()
{

	_rb = AddComponent<Rigidbody2D>();
	_cc = AddComponent<CircleCollider2D>();
	_pc = AddComponent<PlayerController>();

	name = "Player";
}