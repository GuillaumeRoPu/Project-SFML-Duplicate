#pragma once
#include "Components/Scripting.h"
#include <Components/Physics/BoxCollider2D.h>

class PlayerHealthBlocker : public Scripting
{
public:
	int rotationSpeed = 360.0f;
	int angle = 0;
	float rad = 0;
public:
	void Rotate(bool dir);
};