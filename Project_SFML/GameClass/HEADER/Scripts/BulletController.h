#pragma once
#include "Components/Scripting.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components/Physics/Rigidbody2D.h"

class Player;

class BulletController : public Scripting 
{
	void FixedUpdate() override;
	void Awake() override;
	void OnTriggerEnter2D(Collider2D* other) override;
	void OnCollisionEnter2D(Collider2D* other) override;

public:
	void GetPlayer(Player* player);

public:
	Rigidbody2D* _rb;
	CircleCollider2D* _collider;
	Player* _player;
};