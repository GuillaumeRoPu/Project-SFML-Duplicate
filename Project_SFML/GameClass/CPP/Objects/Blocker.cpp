#include "Objects/Blocker.h"

Blocker::Blocker()
{
	_bc = AddComponent<BoxCollider2D>();
	_phc = AddComponent<PlayerHealthBlocker>();
}

void Blocker::Transformation(sf::Vector2f size, Vector2 pos, float rotation)
{
	transform->setLocalPosition(pos);
	transform->setLocalScale(Vector2{ size.x,size.y });
	transform->rotation = rotation;
}
