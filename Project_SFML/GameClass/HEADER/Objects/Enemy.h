#pragma once
#include "Objects/GameObject.h"
#include "Scripts/EnemyController.h"

class Enemy : public GameObject 
{
public:
	Rigidbody2D* _rb;
	BoxCollider2D* _collider;
	EnemyController* _controller;

public:
	Enemy();
};