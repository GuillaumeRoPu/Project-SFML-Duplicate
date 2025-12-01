#pragma once
#include "Components/Component.h"

class Rigidbody2D : public Component
{
public:

	void Awake() override;
	void FixedUpdate() override;

	bool gravity = false;
	bool autoMass = true;
	float mass = 1;
	float damping = 5;
	Vector2 velocity = Vector2();

	void AddForce(Vector2 force, ForceMode2D type = ForceMode2D::Force);

private:
	Vector2 appliedForce = Vector2();
};