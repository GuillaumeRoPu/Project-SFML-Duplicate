#include "Entity/Entity.h"

Entity::Entity(float hp, float x, float y, float width, float height)
{
	name = "Entity";

	life = life_max = hp;
	transform.position = Vector2(x, y);
	transform.scale = Vector2(width, height);
	speed = 1.f;

	rigidbody = AddComponent<Rigidbody2D>();
	collider = AddComponent<BoxCollider2D>();
	renderer = AddComponent<SpriteRenderer>();

	// Set renderer sprite
	renderer->SetTexture("res/Engine/Sprites/square.png");
}

// Add hp to life
void Entity::AddLife(float toAdd)
{
	life += toAdd;
	life = life > life_max ? life_max :
			life < 0.0f ? 0.0f : life;
}