#pragma once
#include <memory>
#include <Engine\FryEngine.h>
#include "Components/Physics/Collider2D.h"
#include <list>

class CircleCollider2D;
class PolygonCollider2D;

class BoxCollider2D : public Collider2D
{
public:
	void Start() override;

	void ApplyRotation() override;
	void UpdateCollision(std::unique_ptr<GameObject>& collider) override;
	bool isPointInside(Vector2 point) override;
	Vector2 GetProjectInterval(Vector2 normal);

	fry::Rect bounds;
	std::list<Vector2> OBBRect;

private:
	void AABBWithAABB(BoxCollider2D& collider);
	void CircleWithAABB(CircleCollider2D& collider);
	void PolygonWithAABB(PolygonCollider2D& collider);

};