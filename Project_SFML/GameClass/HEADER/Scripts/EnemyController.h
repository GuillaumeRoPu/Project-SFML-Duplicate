#pragma once
#include "Objects/GameObject.h"
#include "Components/Scripting.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Objects/Player.h"
#include "Scene.h"

class EnemyController : public Scripting
{
	void FixedUpdate() override;
	void Update() override;
	void Start() override;
public:
	void GetPlayer(Scene* scene);

public:
	Rigidbody2D* _rb;
	BoxCollider2D* _collider;
	Player* _player;
};
