#pragma once
#include "Components/Scripting.h"


class Player;
class Rigidbody2D;
class CircleCollider2D;

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