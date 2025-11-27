#pragma once
#include "Components/Scripting.h"
#include "Objects/GameObject.h"

class PlayerController : public Scripting
{
public:
	Rigidbody2D* _rb;
	float acceleration = 500.0f;

	void Start() override;
public:
	void Move(int verDir, int horDir);
};