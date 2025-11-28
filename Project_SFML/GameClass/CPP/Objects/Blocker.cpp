#include "Objects/Blocker.h"

Blocker::Blocker()
{
	//_bc = AddComponent<BoxCollider2D>();
	_phc = AddComponent<PlayerHealthBlocker>();
}

void Blocker::Transformation(sf::Vector2f s, Vector2 pos, float rot)
{
	size = s;
	position = pos;
	rotation = rot;
}
