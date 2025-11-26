#pragma once
#include "Objects/GameObject.h"

class EnemyController : public Scripting 
{
	void FixedUpdate() override;

public:
	Rigidbody2D* _rb;
};