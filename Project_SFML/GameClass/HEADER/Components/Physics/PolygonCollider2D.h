#pragma once
#include "Engine/FryEngine.h"
#include "Components/Physics/Collider2D.h"
#include <memory>

class BoxCollider2D;
class CircleCollider2D;

class PolygonCollider2D : public Collider2D
{
public:
	void Start() override;

	void UpdateCollision(std::unique_ptr<GameObject>& collider) override;
	bool isPointInside(Vector2 point) override;

	Vector2 GetProjectInterval(Vector2 normal);

	Vector2 center();

	std::vector<Vector2> vertices;

private:
	void AABBWithPolygon(BoxCollider2D& collider);
	void CircleWithPolygon(CircleCollider2D& collider);
	void PolygonWithPolygon(PolygonCollider2D& collider);
};