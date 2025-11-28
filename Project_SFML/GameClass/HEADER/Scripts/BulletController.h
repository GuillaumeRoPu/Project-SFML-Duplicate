#pragma once
#include "Scene.h"
#include "Objects/Player.h"
#include "Components/Scripting.h"
#include "Scripts/BulletManagerController.h"

class Scene;
class Rigidbody2D;
class CircleCollider2D;
class Collider2D;
class BulletManagerController;

class BulletController : public Scripting 
{
	void FixedUpdate() override;
	void Awake() override;
	void Start() override;
	void OnTriggerEnter2D(Collider2D* other) override;
	void OnCollisionEnter2D(Collider2D* other) override;

public:
	BulletManagerController* controller;
	Rigidbody2D* _rb;
	CircleCollider2D* _collider;
	Player* _player;
};