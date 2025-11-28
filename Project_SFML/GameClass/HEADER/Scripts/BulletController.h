#pragma once
#include "Objects/GameObject.h"
#include "Components/Scripting.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Objects/Player.h"
#include "Scene.h"

class BulletController : public Scripting 
{
	void FixedUpdate() override;
	void Awake() override;
	void OnTriggerEnter2D(Collider2D* other) override;

public:
	void GetPlayer(Player* player);

public:
	Rigidbody2D* _rb;
	CircleCollider2D* _collider;
	Player* _player;
};