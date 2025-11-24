#define _USE_MATH_DEFINES
#include "Entity/Player.h"

Player::Player(float hp, float x, float y, float width, float height) : Entity(hp, x, y, width, height)
{
	name = "Player";
	tag = "Player";
	DestroyComponent(collider);
	collider = AddComponent<PolygonCollider2D>();
	renderer->SetTexture("res/Engine/Sprites/triangle.png");
}