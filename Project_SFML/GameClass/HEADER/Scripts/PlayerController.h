#pragma once
#include "Components/Scripting.h"
#include "Objects/GameObject.h"
#include "Components/Physics/Collider2D.h"
#include "Scripts/HealthComponent.h"
#include "InputSystem.h"

class PlayerController : public Scripting
{
private:
	Rigidbody2D* _rb;
	HealthComponent* _health;
	float _acceleration = 10.0f;

	bool _isDashing = false;
	float _tDashCooldown = 2.0f;
	float _tDashDuration = 0.2f;
	float _tDashTimer = 0.0f;
	float _dashForce = 1500.0f;
	Vector2 _dashDir = { 0.0f,0.0f };

	float acceleration = 10.0f;
	
	int score = 0;
	void OnCollisionEnter2D(Collider2D* other) override;

public:
	void PlayerInputTreatment(InputSystem& input);
	void BounceOnBorder(float borderX, float borderY, int playerSize);
	void Heal(int amount);
	void TakeDamage(int amount);
private:
	void Awake() override;
	void Update() override;
	void Move(Vector2 dir);
	void Dash(Vector2 dir);
	void SpawnDeathParticle();

private:
	// Ralentit
	float _timer = 0.f;
	float _duration = 0.5f;
};