#pragma once
#include "Components/Component.h"
#include "Components/Physics/Rigidbody2D.h"

class Collider2D : public Component
{
public:
	
	virtual void ApplyRotation() {};
	virtual void UpdateCollision(GameObject* collider) {};
	virtual bool isPointInside(Vector2 point) { return false; };
	virtual Vector2 GetProjectInterval(Vector2 normal) { return Vector2::zero(); };
	
	bool findIntersection(Collider2D* c1, Collider2D* c2);

	std::vector<Collider2D*> Collisions() { return _collision; }

	bool isTrigger = false;

protected:
	Rigidbody2D* body = nullptr;
	std::vector<Collider2D*> _collision;
	float _lastRotation = INFINITY;

};