#pragma once
#include "Objects/GameObject.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Scripts/BulletController.h"

class RigidBody2D;
class CircleCollider2D;
class BulletController;

class Bullet : public GameObject 
{
public:
	Rigidbody2D* _rb;
	CircleCollider2D* _collider;
	BulletController* _controller;

public:
	Bullet();
};