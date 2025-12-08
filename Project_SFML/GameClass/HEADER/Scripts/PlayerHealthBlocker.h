#pragma once
#include "Components/Scripting.h"
#include <Components/Physics/BoxCollider2D.h>
#include "InputSystem.h"

class PlayerHealthBlocker : public Scripting
{
public:
	int angle = 0;
private:
	int _rotationSpeed = 360;
	float _healthSize;

public:
	void BlockerInputTreatment(InputSystem& input);
	void SetHP(float ratio);
	float GetHealthSize() { return _healthSize; }
private:
	void Rotate(int dir);
};