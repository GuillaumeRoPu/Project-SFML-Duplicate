#pragma once
#include "Components/Component.h"
#include "Components/Physics/Rigidbody2D.h"

class Collider2D : public Component
{
public:
	
	void Start() override;

	virtual void ApplyRotation() {};
	virtual void UpdateCollision(std::unique_ptr<GameObject>& collider);
	virtual bool isPointInside(Vector2 point);
	bool findIntersection(Collider2D* c1, Collider2D* c2);

	bool isTrigger = false;

protected:
	Rigidbody2D* body = nullptr;
	std::vector<Collider2D*> _collision;
	
};