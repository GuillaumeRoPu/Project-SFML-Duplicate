#pragma once
#include "Objects/GameObject.h"
#include "Scripts/EnemyController.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Scripts/HealthComponent.h"

class Enemy : public GameObject
{
public:
	Rigidbody2D* _rb;
	BoxCollider2D* _collider;
	EnemyController* _controller;
	HealthComponent* _health;

public:
	Enemy();
};