#pragma once
#include <memory>
#include <Engine\FryEngine.h>
#include "Components/Physics/Collider2D.h"

class BoxCollider2D;
class PolygonCollider2D;

class CircleCollider2D : public Collider2D
{
public:
	void Start() override;

	void UpdateCollision(std::unique_ptr<GameObject>& collider) override;
	bool isPointInside(fry::Vector2 point) override;

	float radius = 1.f;

private:
	void AABBWithCircle(BoxCollider2D& collider);
	void CircleWithCircle(CircleCollider2D& collider);
	void PolygonWithCircle(PolygonCollider2D& collider);

};