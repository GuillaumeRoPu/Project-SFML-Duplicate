#pragma once
#include "Components/Scripting.h"
#include "Objects/GameObject.h"
#include "Components/Physics/Collider2D.h"
#include "Scripts/HealthComponent.h"

class PlayerController : public Scripting
{
public:
	Rigidbody2D* _rb;
	HealthComponent* _health;
	float acceleration = 10.0f;

	void Awake() override;
	void Update() override;
public:
	void Move(int verDir, int horDir);
	void Heal(int amount);
	void TakeDamage(int amount);
};