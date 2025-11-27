#pragma once
#include "Objects/GameObject.h"
#include "Components/Scripting.h"
#include "Components/Physics/BoxCollider2D.h"

class EnemyController : public Scripting
{
	void FixedUpdate() override;
	void Start() override;

public:
	Rigidbody2D* _rb;
	BoxCollider2D* _collider;
};
