#pragma once
#include "Objects/GameObject.h"
#include "Scripts/PlayerController.h"
#include "Scripts/HealthComponent.h"

class Rigidbody2D;
class CircleCollider2D;

class Player : public GameObject
{
public:
	Player();
public:
	Rigidbody2D* _rb;
	CircleCollider2D* _cc;
	PlayerController* _pc;
	HealthComponent* _health;
};