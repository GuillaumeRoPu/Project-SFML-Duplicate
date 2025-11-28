#pragma once
#include "Objects/GameObject.h"
#include "Scripts/EnemyController.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Scripts/BulletController.h"

class Bullet : public GameObject 
{
public:
	Rigidbody2D* _rb;
	CircleCollider2D* _collider;
	BulletController* _controller;

public:
	Bullet();
};