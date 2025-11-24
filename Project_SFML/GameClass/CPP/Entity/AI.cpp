#include <Entity/AI.h>

AI::AI(float hp, float x, float y, float width, float height, float strength) : Entity(hp, x, y, width, height)
{
	this->strength = strength;
	name = "Enemy";

	active = false;
}

void AI::SetTargetPosition(Vector2 target)
{
	if (!reachTarget)
		return;

	Vector2 dir(target.x() - transform.position.x(), target.y() - transform.position.y());
	(*rigidbody).velocity = dir.normalized();
}
