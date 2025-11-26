#pragma once
#include "Objects/GameObject.h"
#include "Components/Scripting.h"

class EnemyController : public Scripting
{
	void FixedUpdate() override;

public:
	Rigidbody2D* _rb;
};