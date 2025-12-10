#pragma once
#include "Objects/GameObject.h"
#include "Scripts/EnemyController.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Scripts/HealthComponent.h"

class Enemy : public GameObject
{
public:
	Rigidbody2D* _rb;
	CircleCollider2D* _collider;
	EnemyController* _controller;
	HealthComponent* _health;

public:
	Enemy();
};